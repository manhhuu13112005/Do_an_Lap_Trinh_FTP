#ifndef __FTP_SERVER_H__
#define __FTP_SERVER_H__

#include "tcpserver.h"
#include "tcpsocket.h"
#include "MyServerConfig.h"
#include "FTPSession.h"
#include <iostream>

//class EchoTCPServer: public TCPServer
//{
//public:
//    EchoTcpServer(unsigned short port =21);
//    ~EchoTcpServer();
//protected:
//    virtual void startNewSession(TcpSocket slave);
//    int doEcho(TcpSocket& slave);
//};

class FTPServer : public TCPServer {
public:
    FTPServer(unsigned short port =21);//Lắng nghe port đã định.
    ~FTPServer();

protected:
    //Khi client kết nối, TCPServer gọi startNewSession() với socket client.
    virtual void startNewSession(TcpSocket slave);
     int doEcho(TcpSocket& slave);
     //Nhận dữ liệu client gửi.
        //Gửi lại dữ liệu đó (echo).
private:
    void handleClient(TcpSocket slave);
    int readLine(TcpSocket &s, char* buf, int maxLen);  // <-- thêm dòng này
    MyServerConfig serverConf;

};

#endif
