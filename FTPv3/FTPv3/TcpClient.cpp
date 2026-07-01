#include "TcpClient.h"
#include <iostream>
#include <fstream>

TcpClient::TcpClient() {
    connected = false;
}

TcpClient::~TcpClient() {
}

bool TcpClient::open(const string& serverHost, unsigned short port) {
    try {
        this->localsocket.connect(serverHost, port);
        this->connected = true;
        return true;
    } catch(SocketException& e) {
        cerr << e.what() << endl;
        return false;
    }
}

bool TcpClient::open(const string& serverHost, const string& port) {
    try {
        this->localsocket.connect(serverHost, port);
        this->connected = true;
        return true;
    } catch(SocketException& e) {
        cerr << e.what() << endl;
        return false;
    }
}

int TcpClient::sendData(const char* buffer, unsigned int len) {
    // Gọi sendDataBuffer đã có
    return sendDataBuffer(buffer, len);
}

int TcpClient::sendDataBuffer(const char* buffer, unsigned int bufLen) {
    try {
        return this->localsocket.send(buffer, bufLen);
    } catch(SocketException& e) {
        cerr << e.what() << endl;
        return -1;
    }
}

int TcpClient::sendStringRequest(const string& msg)
{
    int total = 0;
    size_t msgLen = msg.size(); // dùng size_t cho an toàn
    const char* buf = msg.c_str();

    while (total < (int)msgLen) {
        int s = localsocket.send(buf + total, (int)(msgLen - total));
        if (s <= 0) return total;  // nếu lỗi hoặc ngắt kết nối
        total += s;
    }
    return total;
}

int TcpClient::recvGetLine(char* buffer, unsigned int maxLen)
{
    int bytes;
    try
    {
        // truyền toàn bộ kích thước để recvLine tự bảo vệ
        bytes = localsocket.recvLine(buffer, (int)maxLen);
        if(bytes > 1) // có thể có CRLF
        {
            // nếu line kết thúc bằng \r\n thì đặt terminator trước \r
            if (bytes >= 2 && buffer[bytes-2] == '\r' && buffer[bytes-1] == '\n')
            {
                buffer[bytes-2] = 0;
            }
            else
            {
                // đặt null terminator ở cuối
                if ((unsigned)bytes < maxLen) buffer[bytes] = 0;
                else buffer[maxLen-1] = 0;
            }
        }
        else
        {
            // không đọc được gì
            if (maxLen > 0) buffer[0] = 0;
        }
        return bytes;
    }
    catch(SocketException& e)
    {
        cerr << e.what() << endl;
        return -1;
    }
}


int TcpClient::recvDataBuffer(char* buffer, unsigned int bufLen) {
    int bytes;
    try {
        // Đọc đúng số byte yêu cầu
        bytes = localsocket.recv(buffer, bufLen);
        return bytes;
    } catch(SocketException& e) {
        cerr << e.what() << endl;
        return -1;
    }
}



int TcpClient::recvStringBuffer(char* buffer, unsigned int bufLen) {
    int bytes;
    try {
        bytes = localsocket.recv(buffer, bufLen - 1);
        if (bytes > 0) {
            buffer[bytes] = 0;
        } else {
            *buffer = 0;
        }
        return bytes;
    } catch(SocketException& e) {
        cerr << e.what() << endl;
        *buffer = 0;
        return -1;
    }
}

void TcpClient::close() {
    try {
        localsocket.shutdown(SHUTDOWN_BOTH);
        localsocket.close();
    } catch(SocketException& e) {
        cerr << e.what() << endl;
    }
    connected = false;
}

void TcpClient::printLn(const string& msg) {
    cout << msg << endl << endl;
}

void TcpClient::print(const string& msg) {
    cout << msg << endl;
}
