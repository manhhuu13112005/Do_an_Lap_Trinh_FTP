#ifndef ECHOTCPCLIENT_H
#define ECHOTCPCLIENT_H
#include "TcpClient.h"

class EchoTcpClient: public TcpClient
{
    public:
        EchoTcpClient();

        string echo(const string& msg);
        string echo();

        bool sendLine(const string &line);
    string recvLine();
    bool login(const string &ip, const string &user, const string &pass);
    bool submitFile(const string &filePath);
    void quit();

    // Gửi chuỗi lệnh (USER, PASS, QUIT, SUBMIT...)
    int sendCmd(const string& request) {
        return sendStringRequest(request);   // gọi hàm protected của TcpClient
    }

    // Gửi data file
    int sendRaw(const char* buf, unsigned int n) {
        return sendData(buf, n);
    }

    protected:

    private:
        string myMsg = "Hello, I am 68MHT1 Echo CLient";
};

#endif // ECHOTCPCLIENT_H
// gui va nhan thong diep tu server
