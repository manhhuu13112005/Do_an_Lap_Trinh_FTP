#ifndef FTPSESSION_H_
#define FTPSESSION_H_

#include "session.h"
#include <string>
#include <filesystem>
#include <sstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

class FTPSession : public Session {
public:
    FTPSession(const TcpSocket& slave, ServerConfig* conf);
    ~FTPSession() override;

    void run(); // vòng lặp chính của phiên
    void doUnknown(std::string argv[], int argc) override;

private:
    bool loggedIn = false;
    std::string currentUser;

    // tiện ích
    int readLine(TcpSocket &s, char* buf, int maxLen);
    void sendResponse(const std::string& resp);

    // các lệnh
    void doLogin(std::string argv[], int argc);
    void doSubmit(std::string argv[], int argc);
    void doQuit(std::string argv[], int argc);

    // hỗ trợ SUBMIT
    bool parseSubmitLine(const std::string& cmdLine, std::string& filename, size_t& filesize);
    bool receiveFile(const std::string& filename, size_t filesize);
};

#endif
