#ifndef __APP_PROTOCOL_H__
#define __APP_PROTOCOL_H__

#include "socket.h"
#include <string>

using namespace std;

class AppProtocol {
public:
    static void sendMessage(Socket* sock, const string& message);
    static string receiveMessage(Socket* sock);
};

#endif
