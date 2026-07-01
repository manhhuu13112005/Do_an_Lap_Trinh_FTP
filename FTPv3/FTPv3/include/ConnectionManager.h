#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include "socket.h"
#include <string>

using namespace std;

class ConnectionManager {
private:
    Socket* clientSocket;     // dùng lớp Socket trong thư viện của bạn
    string serverAddress;
    unsigned short serverPort;

public:
    ConnectionManager(const string& address, unsigned short port);
    ~ConnectionManager();

    void connectToServer();   // Bước 1–3: tạo, kết nối socket
    void disconnect();        // Bước 5: đóng socket
    Socket* getSocket();      // trả socket cho module thực thi sử dụng
};

#endif
