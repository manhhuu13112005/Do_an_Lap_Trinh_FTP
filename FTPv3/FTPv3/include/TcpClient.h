#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "tcpsocket.h"

class TcpClient
{
protected:
    TcpSocket localsocket;
    bool connected;

public:
     TcpClient();                       // Constructor, khởi tạo đối tượng TcpClient
    ~TcpClient();                      // Destructor, hủy đối tượng TcpClient
    bool isConnected() { return connected;} // Kiểm tra xem có kết nối hay không
    bool open(const string& serverHost, unsigned short port);  // Mở kết nối tới server qua host và port
    bool open(const string& serverHost, const string& port);   // Mở kết nối với server qua host và port dưới dạng string
    void close();
    std::string recvLine();                   // Đóng kết nối
    int sendData(const char* buffer, unsigned int len);
    int sendStringRequest(const string& request);
protected:

    int sendDataBuffer(const char* buffer, unsigned int bufLen);
    int recvGetLine(char* buf,unsigned int maxLen);
    int recvDataBuffer(char* buffer, unsigned int bufLen);
    int recvStringBuffer(char* buffer, unsigned int bufLen);
    void print(const string& msg);
    void printLn(const string& msg);


private:
    void startGradingMode();
    void stopGradingMode();
};
#endif // _TCP_CLIENT_H_
