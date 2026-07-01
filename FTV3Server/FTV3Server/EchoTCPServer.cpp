#include "EchoTCPServer.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

FTPServer::FTPServer(unsigned short port) : TCPServer(port)
{
   serverConf.loadAccountsFromFile("D:/DOCUMENT/DoAnLTHTM/FTV3Server/users.txt");
}

FTPServer::~FTPServer() {}

void FTPServer::startNewSession(TcpSocket slave)
{
    std::thread sessionThread([this, s = std::move(slave)]() mutable {
        // tạo FTPSession cho kết nối này
        FTPSession session(std::move(s), &serverConf);
        session.run();
    });
    sessionThread.detach();
}

void FTPServer::handleClient(TcpSocket slave)
{
    // chạy session trên thread riêng
    FTPSession session(std::move(slave), &serverConf);
    session.run();
}



// =========================
// Hàm đọc 1 dòng CRLF
// =========================
int FTPServer::readLine(TcpSocket &s, char* buf, int maxLen)
{
    int total = 0;
    char c;

    while(total < maxLen - 1) {
        int r = s.recv(&c, 1);
        if(r < 0) return -1; // lỗi
        if(r == 0) {          // client đóng
            if(total == 0) return 0;
            break;
        }

        buf[total++] = c;

        if(total >= 2 && buf[total-2] == '\r' && buf[total-1] == '\n')
            break;
    }

    buf[total] = 0;
    return total;
}
