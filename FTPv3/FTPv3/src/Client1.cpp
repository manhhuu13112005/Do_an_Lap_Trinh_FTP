#include "tcpserver.h"
#include <iostream>
using namespace std;

int main() {
    TCPServer server(8080);
    cout << "FTP Server đang lắng nghe tại cổng 8080..." << endl;
    while (true) {
        TCPSocket* client = server.acceptClient();
        if (client) {
            cout << "Client kết nối thành công!\n";
            // TODO: Nhận file từ client
            delete client;
        }
    }
    return 0;
}
