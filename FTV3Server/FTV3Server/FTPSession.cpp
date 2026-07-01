#include "FTPSession.h"
#include <fstream>

using namespace std;

FTPSession::FTPSession(const TcpSocket& s, ServerConfig* c)
    : Session(s, c) {}

FTPSession::~FTPSession() {}

void FTPSession::sendResponse(const std::string& resp) {
    cout << "[Server] Sending response: " << resp << endl;
    slave.send(resp.c_str(), resp.size());
}

int FTPSession::readLine(TcpSocket &s, char* buf, int maxLen) {
    int total = 0;
    char c;
    while (total < maxLen - 1) {
        int r = s.recv(&c, 1);
        if (r < 0) return -1;
        if (r == 0) {
            if (total == 0) return 0;
            break;
        }
        buf[total++] = c;
        if (total >= 2 && buf[total-2] == '\r' && buf[total-1] == '\n') break;
    }
    buf[total] = 0;
    return total;
}

void FTPSession::doUnknown(std::string argv[], int argc) {
    sendResponse("502 FAIL\r\n");
}

void FTPSession::doLogin(std::string argv[], int argc) {
    if (argc < 4) {
        sendResponse("501 Syntax error in parameters\r\n");
        return;
    }
    std::string ip = argv[1], username = argv[2], password = argv[3];
    std::string realIp = slave.getRemoteAddress();
    std::cout << "[Server] Login attempt from " << realIp << " as user '" << username << "'\n";

    if (conf->authenticate(username, password)) {
        loggedIn = true;
        currentUser = username;
        sendResponse("230 OK\r\n");
    } else {
        loggedIn = false;
        currentUser.clear();
        sendResponse("530 FAIL\r\n");
    }
}

bool FTPSession::parseSubmitLine(const std::string& cmdLine, std::string& filename, size_t& filesize) {
    // cmdLine dạng: "SUBMIT <filename possibly with spaces> <size>"
    std::istringstream iss(cmdLine);
    std::string token; iss >> token; // SUBMIT

    std::string rest;
    std::getline(iss, rest); // phần còn lại
    auto ltrim = [](std::string &s){ s.erase(0, s.find_first_not_of(" \t")); };
    auto rtrim = [](std::string &s){ if (!s.empty()) s.erase(s.find_last_not_of(" \t") + 1); };
    ltrim(rest); rtrim(rest);

    size_t lastSpace = rest.find_last_of(' ');
    if (lastSpace == std::string::npos) return false;

    filename = rest.substr(0, lastSpace);
    std::string sizeStr = rest.substr(lastSpace + 1);
    ltrim(filename); rtrim(filename);
    ltrim(sizeStr);  rtrim(sizeStr);

    try {
        filesize = static_cast<size_t>(std::stoll(sizeStr));
    } catch (...) {
        return false;
    }
    return !filename.empty() && filesize > 0;
}

bool FTPSession::receiveFile(const std::string& filename, size_t filesize) {
    // chuẩn bị thư mục
    fs::path storage("server_storage");
    if (!fs::exists(storage)) fs::create_directory(storage);
    fs::path userDir = storage / currentUser;
    if (!fs::exists(userDir)) fs::create_directories(userDir);

    fs::path outPath = userDir / filename;

    // mở file đích
    ofstream ofs(outPath, ios::binary);
    if (!ofs.is_open()) {
        sendResponse("550 Cannot open file\r\n");
        return false;
    }

    // nhận dữ liệu
    size_t received = 0;
    char buffer[16384];
    const int MAX_RETRY = 5;
    int retryCount = 0;

    while (received < filesize) {
        size_t need = std::min(static_cast<size_t>(sizeof(buffer)), filesize - received);
        int r = -1;
        try {
            r = slave.recv(buffer, static_cast<int>(need));
        } catch (SocketException& ex) {
            cerr << "[WARN] recv exception: " << ex.what()
                 << " at " << received << " / " << filesize << " bytes\n";
            if (++retryCount <= MAX_RETRY) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            // quá retry
            ofs.close();
            sendResponse("426 Connection closed; transfer aborted\r\n");
            return false;
        }

        if (r <= 0) {
            ofs.close();
            sendResponse("426 Connection closed; transfer aborted\r\n");
            return false;
        }

        ofs.write(buffer, r);
        received += static_cast<size_t>(r);
        retryCount = 0;
    }

    ofs.close();
    cout << "[DEBUG] Received " << received << " / " << filesize
         << " bytes for file '" << filename << "'\n";

    if (received == filesize) {
        sendResponse("226 OK\r\n");
        return true;
    } else {
        // tùy chọn: xóa file dang dở
        // fs::remove(outPath);
        return false;
    }
}

void FTPSession::doSubmit(std::string argv[], int argc) {
    if (!loggedIn) {
        sendResponse("530 FAIL\r\n");
        return;
    }

    // Không gửi 150 trước khi parse
    std::string cmdLine;
    {
        // reconstruct original line from argv để parse an toàn
        // nhưng vì ta có sẵn line ở run(), tốt nhất chuyền nguyên line vào đây
        // Để đơn giản: ghép lại
        std::ostringstream oss;
        for (int i = 0; i < argc; ++i) {
            if (i) oss << ' ';
            oss << argv[i];
        }
        cmdLine = oss.str();
    }

    std::string filename;
    size_t filesize = 0;
    if (!parseSubmitLine(cmdLine, filename, filesize)) {
        sendResponse("501 Invalid filename or size\r\n");
        return;
    }

    // parse OK → gửi 150
    sendResponse("150 OK\r\n");

    // nhận file
    if (!receiveFile(filename, filesize)) {
        // phiên upload thất bại: đóng phiên để tránh lệch nhịp
        slave.close();
        quitSession = true;
        return;
    }
}

void FTPSession::doQuit(std::string argv[], int argc) {
    quitSession = true;
    sendResponse("221 BYE\r\n");
}

void FTPSession::run() {
    try {
        // Chào mừng
        sendResponse("220 OK\r\n");

        char lineBuf[1024];

        while (!quitSession) {
            int len = readLine(slave, lineBuf, sizeof(lineBuf));
            if (len <= 0) break;

            std::string cmd(lineBuf);
            if (cmd.size() >= 2 && cmd.substr(cmd.size()-2) == "\r\n")
                cmd = cmd.substr(0, cmd.size()-2);

            cout << "[Server] Raw command: '" << cmd << "'" << endl;

            // parse argv
            std::istringstream iss(cmd);
            std::string token;
            iss >> token;

            // tách toàn bộ thành mảng argv
            std::vector<std::string> parts;
            {
                std::string temp;
                iss.clear();
                std::istringstream iss2(cmd);
                while (iss2 >> temp) parts.push_back(temp);
            }

            // chuyển vào mảng C-style
            std::string argvVec[SERVER_CMD_ARG_NUM];
            int argc = 0;
            for (auto &p : parts) {
                if (argc >= SERVER_CMD_ARG_NUM) break;
                argvVec[argc++] = p;
            }

            // dispatch
            if (token == "login") {
                doLogin(argvVec, argc);
            } else if (token == "SUBMIT") {
                doSubmit(argvVec, argc);
            } else if (token == "QUIT") {
                doQuit(argvVec, argc);
            } else {
                doUnknown(argvVec, argc);
            }
        }

        slave.close();
        cout << "[Client disconnected]\n";
    } catch (SocketException& e) {
        cerr << "[Error] Exception in session: " << e.what() << endl;
        slave.close();
    }
}
