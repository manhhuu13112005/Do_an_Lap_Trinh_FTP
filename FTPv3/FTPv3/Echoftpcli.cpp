#include "Echoftpcli.h"
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

static const string DEFAULT_UPLOAD_FOLDER = "upload_client";

FTPCLI::FTPCLI() : CmdLineInterface("FTP>") {
    initCmd();
}

FTPCLI::~FTPCLI() {
    cout << "Thoat FTP Client.\n";
}

/* ============================================================
                        ĐĂNG KÝ LỆNH
============================================================ */
void FTPCLI::initCmd() {
    addCmd("help",  CLI_CAST(&FTPCLI::cmdHelp));
    addCmd("login", CLI_CAST(&FTPCLI::cmdLogin));
    addCmd("submit",CLI_CAST(&FTPCLI::cmdSubmit));
    addCmd("quit",  CLI_CAST(&FTPCLI::cmdQuit));
}

/* ============================================================
                        LỆNH LOGIN
   login <ip> <username> <password>
============================================================ */
void FTPCLI::cmdLogin(string cmd_argv[], int cmd_argc)
{
    if (cmd_argc != 4) {
        cout << "Cu phap: login <ip> <username> <password>\n";
        return;
    }

    string ip   = cmd_argv[1];
    string user = cmd_argv[2];
    string pass = cmd_argv[3];

    if (!client.open(ip, 21)) {
        cout << "Khong the ket noi den server!\n";
        return;
    }

    if (client.login(ip, user, pass)) {
        cout << "Dang nhap thanh cong!\n";
    } else {
        cout << "Login FAILED.\n";
    }
}

/* ============================================================
                        LỆNH SUBMIT
   submit <filename>   - Upload một file cụ thể trong upload_client
   submit              - Upload tất cả file trong upload_client
   submit all          - Upload tất cả file trong upload_client
============================================================ */
void FTPCLI::cmdSubmit(string cmd_argv[], int cmd_argc)
{
    if (!client.isConnected()) {
        cout << "Chua login!\n";
        return;
    }

    if (!fs::exists(DEFAULT_UPLOAD_FOLDER)) {
        cout << "Thu muc " << DEFAULT_UPLOAD_FOLDER << " khong ton tai!\n";
        return;
    }

    // Nếu người dùng gõ "submit <filename>"
    if ((cmd_argc >= 2 && cmd_argv[1] != "all") && cmd_argc <= 3) {

        string filename = cmd_argv[1];
        fs::path filePath = fs::path(DEFAULT_UPLOAD_FOLDER) / filename;

        if (!fs::exists(filePath)) {
            cout << "File khong ton tai: " << filePath << endl;
            return;
        }

        uintmax_t size = fs::file_size(filePath);
        cout << "Upload mot file: " << filename << " (" << size << " bytes)\n";

        client.sendLine("SUBMIT " + filename + " " + to_string(size));
        string reply = client.recvLine();
        cout << "<-- " << reply << endl;

        if (reply.substr(0, 3) != "150") {
            cout << "Server khong chap nhan file.\n";
            return;
        }

        ifstream f(filePath, ios::binary);
        char buffer[4096];
        size_t sent = 0;
                while (sent < size) {
            f.read(buffer, sizeof(buffer));
            size_t n = f.gcount();
            if (n <= 0) break;

            bool ok = client.sendData(buffer, n);
            if (!ok) {
                cout << "[ERROR] Send failed at " << sent << " / " << size << " bytes\n";
                break; // dừng gửi file này
            }

            sent += n;
        }


        reply = client.recvLine();
        cout << "<-- " << reply << endl;
        return;
    }

    // Nếu người dùng gõ "submit" hoặc "submit all"
    cout << "Upload tat ca file tu thu muc: " << DEFAULT_UPLOAD_FOLDER << endl;
    for (auto &p : fs::directory_iterator(DEFAULT_UPLOAD_FOLDER)) {
        if (!p.is_regular_file()) continue;

        string filename = p.path().filename().string();
        uintmax_t size = fs::file_size(p.path());

        cout << "Upload: " << filename << " (" << size << " bytes)\n";

        client.sendLine("SUBMIT " + filename + " " + to_string(size));
        string reply = client.recvLine();
        cout << "<-- " << reply << endl;

        if (reply.substr(0, 3) != "150") {
            cout << "Server khong chap nhan file.\n";
            continue;
        }

                   ifstream f(p.path(), ios::binary);
            char buffer[4096];
            size_t sent = 0;

            while (sent < size) {
                f.read(buffer, sizeof(buffer));
                size_t n = f.gcount();
                if (n <= 0) break;

                bool ok = client.sendData(buffer, n);
                if (!ok) {
                    cout << "[ERROR] Send failed at " << sent << " / " << size << " bytes\n";
                    break;
                }

                sent += n;
            }

            cout << "[DEBUG] Total sent: " << sent << " / " << size << " bytes\n";


        reply = client.recvLine();
        cout << "<-- " << reply << endl;
    }

    cout << "Hoan tat upload.\n";
}

/* ============================================================
                        LỆNH QUIT
============================================================ */
void FTPCLI::cmdQuit(string cmd_argv[], int cmd_argc)
{
    if (client.isConnected()) {
        client.sendLine("QUIT");
        cout << "<-- " << client.recvLine() << endl;
        client.close();
    }
    cout << "Bye!\n";
    exit(0);
}

/* ============================================================
                        HELP
============================================================ */
void FTPCLI::cmdHelp(string cmd_argv[], int cmd_argc)
{
    cout << "==== DANH SACH LENH FTP ====\n";
    cout << "login <ip> <username> <password>\n";
    cout << "submit <filename>   - Upload mot file trong upload_client\n";
    cout << "submit              - Upload tat ca file trong upload_client\n";
    cout << "submit all          - Upload tat ca file trong upload_client\n";
    cout << "quit                - Thoat\n";
}
