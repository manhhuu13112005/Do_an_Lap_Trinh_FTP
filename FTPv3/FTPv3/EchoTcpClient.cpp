#include "EchoTcpClient.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

EchoTcpClient::EchoTcpClient()
    : TcpClient()
{
}

/* -------------------------------------------------
   Gửi 1 dòng có CRLF
--------------------------------------------------*/
bool EchoTcpClient::sendLine(const string &line)
{
    string msg = line + "\r\n";
    int sent = this->sendStringRequest(msg);
    cout << "[Client] Sending: " << msg << endl;

    if (sent != (int)msg.size())
        cout << "[Client] sendLine FAILED: " << line << endl;

    return sent == (int)msg.size();
}

/* -------------------------------------------------
   Nhận 1 dòng CRLF từ server
--------------------------------------------------*/
string EchoTcpClient::recvLine()
{
    string s;
    char c;
    while(true)
    {
        int r = this->recvDataBuffer(&c, 1);
        if(r <= 0)
        {
            // server đóng kết nối hoặc lỗi
            if(s.empty()) return "";
            break;
        }
        if(c == '\r') continue;
        if(c == '\n') break;
        s.push_back(c);
    }
    return s;
}


/* -------------------------------------------------
   LOGIN: USER + PASS chuẩn FTP
--------------------------------------------------*/
bool EchoTcpClient::login(const string &ip, const string &user, const string &pass)
{
    string welcome = recvLine();
    cout << "[Client] Welcome from server: " << welcome << endl;

    string cmd = "login " + ip + " " + user + " " + pass;
    cout << "[Client] -> " << cmd << endl;
    sendLine(cmd);

    string resp = recvLine();
    cout << "<-- " << resp << endl;

    return resp.substr(0, 3) == "230";
}


/* -------------------------------------------------
   SUBMIT file lên server
--------------------------------------------------*/
bool EchoTcpClient::submitFile(const string &filePath)
{
    ifstream f(filePath, ios::binary);
    if (!f.is_open())
    {
        cout << "Khong mo duoc file: " << filePath << endl;
        return false;
    }

    size_t pos = filePath.find_last_of("/\\");
    string filename = (pos == string::npos) ? filePath : filePath.substr(pos + 1);

    // Tính size file
    f.seekg(0, ios::end);
    long size = f.tellg();
    f.seekg(0, ios::beg);

    // Gửi SUBMIT
    string cmd = "SUBMIT " + filename + " " + to_string(size);
    cout << "[Client] -> " << cmd << endl;
    sendLine(cmd);

    // Nhận 150
    string resp = recvLine();
    cout << "<-- " << resp << endl;

    if (resp.substr(0, 3) != "150")
    {
        cout << "Server khong chap nhan SUBMIT" << endl;
        return false;
    }

    // Gửi file binary
    char buffer[4096];
    long sent = 0;

    while (sent < size)
{
    f.read(buffer, sizeof(buffer));
    int bytes = f.gcount();
    if (bytes <= 0) break;

    bool ok = this->sendData(buffer, bytes);
    if (!ok) {
        cout << "[ERROR] Send failed at " << sent << " / " << size << " bytes\n";
        return false; // dừng ngay nếu lỗi
    }

    sent += bytes;
}


    // Nhận 226
    resp = recvLine();
    cout << "<-- " << resp << endl;

    return resp.substr(0, 3) == "226";
}

/* -------------------------------------------------
   QUIT
--------------------------------------------------*/
void EchoTcpClient::quit()
{
    sendLine("QUIT");
    string resp = recvLine();
    cout << "<-- " << resp << endl;
}
