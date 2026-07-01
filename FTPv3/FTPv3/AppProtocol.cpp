#include "AppProtocol.h"
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#endif

void AppProtocol::sendMessage(Socket* sock, const string& message) {
    int bytesSent = ::send(sock->sockDesc, message.c_str(), message.size(), 0);
    if (bytesSent < 0) {
        throw SocketException("Gửi dữ liệu thất bại (send())", true);
    }
    cout << "📤 Đã gửi: " << message << endl;
}

string AppProtocol::receiveMessage(Socket* sock) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRecv = ::recv(sock->sockDesc, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecv < 0) {
        throw SocketException("Nhận dữ liệu thất bại (recv())", true);
    }
    string response(buffer);
    cout << "📥 Nhận từ server: " << response << endl;
    return response;
}
