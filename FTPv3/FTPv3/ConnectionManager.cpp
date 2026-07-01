#include "ConnectionManager.h"
#include <iostream>

ConnectionManager::ConnectionManager(const string& address, unsigned short port)
    : serverAddress(address), serverPort(port), clientSocket(nullptr) {}

ConnectionManager::~ConnectionManager() {
    disconnect();
}

void ConnectionManager::connectToServer() {
    try {
        // 1. Tạo socket TCP
        clientSocket = new Socket(SOCK_STREAM, IPPROTO_TCP);

        // 2. Tạo cấu trúc địa chỉ server
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());

        // 3. Kết nối đến server
        if (::connect(clientSocket->sockDesc, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            throw SocketException("Kết nối server thất bại (connect())", true);
        }

        cout << "✅ Đã kết nối đến server " << serverAddress << ":" << serverPort << endl;
    }
    catch (SocketException& e) {
        cerr << "❌ Lỗi kết nối: " << e.what() << endl;
        delete clientSocket;
        clientSocket = nullptr;
    }
}

void ConnectionManager::disconnect() {
    if (clientSocket != nullptr) {
        clientSocket->close();
        delete clientSocket;
        clientSocket = nullptr;
        cout << "🔌 Đã ngắt kết nối với server.\n";
    }
}

Socket* ConnectionManager::getSocket() {
    return clientSocket;
}
