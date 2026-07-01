#include "tcpserver.h"
#include <iostream>
#include <exception>

TCPServer::TCPServer(unsigned short localPort)
{
    this->port = localPort;
    running = false;
    autoStart = false;
    numCmd = 0;
}

TCPServer::~TCPServer() {}

void TCPServer::initCmd() {}

bool TCPServer::configServer() { return true; }
bool TCPServer::loadServerConfig(const string& confFileName) { return true; }

void TCPServer::addCmd(const string& cmdName, CMD_FUNC f)
{
    if(numCmd < SERVER_CMD_MAX_NUM) {
        cmdNameList[numCmd] = cmdName;
        cmdDoFunc[numCmd] = f;
        numCmd++;
    }
}

void TCPServer::doCmd(Session* session, uint8_t cmdId, string cmd_argv[], int cmd_argc)
{
    if(cmdId == SERVER_CMD_UNKNOWN) session->doUnknown(cmd_argv, cmd_argc);
    else if(session && cmdDoFunc[cmdId]) (session->*cmdDoFunc[cmdId])(cmd_argv, cmd_argc);
}

bool TCPServer::start() {
    try {
        master.setListen(port); // bind + listen
        running = true;
        listenThread = std::thread([this]() { this->acceptLoop(); });
        listenThread.detach();
    } catch (SocketException& e) {
        std::cerr << "[TCPServer] Failed to start: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void TCPServer::stop() {
    running = false;
    master.close();
}

bool TCPServer::restart() {
    stop();
    try {
        master.setListen(this->port);
        running = true;
        std::thread t(&TCPServer::run, this);
        t.detach();
        return true;
    } catch(std::exception& e) {
        std::cerr << "[TCPServer] restart error: " << e.what() << std::endl;
        return false;
    }
}

void TCPServer::run() {
    while(running) {
        try {
            TcpSocket slave = master.accept();
            if(conf) {
                int timeout = conf->getTimeout();
                if(timeout) slave.setTimeOut(timeout);
            }

#ifdef _CONCURRENT_MT_SERVER_
            std::thread t(&TCPServer::startNewSession, this, slave);
            t.detach();
#else
            startNewSession(slave);
#endif
        } catch(std::exception& e) {
            if(!running) return;
            std::cerr << "[TCPServer] accept error: " << e.what() << std::endl;
        }
    }
}

void TCPServer::acceptLoop() {
    while(running) {
        try {
            TcpSocket clientSocket = master.accept();
            startNewSession(clientSocket);
        } catch(std::exception &e) {
    if(!running) return; // nếu server đang stop thì không cần báo lỗi
    std::cerr << "[TCPServer] Accept error: " << e.what() << std::endl;
}
    }
}
bool TCPServer::isRunning() {
    return running;
}
