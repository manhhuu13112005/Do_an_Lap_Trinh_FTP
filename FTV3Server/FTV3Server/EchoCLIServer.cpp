#include "EchoCLIServer.h"
#include "EchoTCPServer.h"

using namespace std;

FTPServerCLI::FTPServerCLI():CmdLineInterface("server>")
{
    EchoTcpServer = new FTPServer(21); // tao doi tuong
    initCmd();
}
FTPServerCLI::~FTPServerCLI(){
    delete EchoTcpServer; // xoa doi tuong
}
void FTPServerCLI::initCmd()
{
    addCmd("help",  CLI_CAST(&FTPServerCLI::cmdHelp));
    addCmd("start", CLI_CAST(&FTPServerCLI::cmdStart));
    addCmd("stop",  CLI_CAST(&FTPServerCLI::cmdStop));
    addCmd("status",CLI_CAST(&FTPServerCLI::cmdStatus));
}

// lệnh START server
void FTPServerCLI::cmdStart(string argv[], int argc)
{
    if (EchoTcpServer->isRunning())
        cout << "Server dang chay roi!\n";
    else {
        // Khởi động server (port 21)
        if (EchoTcpServer->start())  // giả sử start() có thể nhận port
            cout << ">>> FTP Server dang lang nghe tren cong 21...\n";
        else
            cout << "[LOI] Khong the khoi dong FTP Server!\n";
    }
}
// lệnh STOP server
void FTPServerCLI::cmdStop(string argv[], int argc)
{
    if (!EchoTcpServer->isRunning())
        cout << "Server chua chay!\n";
    else {
        EchoTcpServer->stop();
        cout << ">>> FTP Server da dung.\n";
    }
}

// lệnh STATUS server
void FTPServerCLI::cmdStatus(string argv[], int argc)
{
    cout << "Trang thai server: "
         << (EchoTcpServer->isRunning() ? "DANG CHAY" : "DANG TAT") << "\n";
}
// lệnh HELP
void FTPServerCLI::cmdHelp(string argv[], int argc)
{
    cout << "\nDanh sach lenh ho tro:\n";
    cout << "  help       - Hien thi tro giup\n";
    cout << "  start      - Khoi dong FTP server\n";
    cout << "  stop       - Dung server\n";
    cout << "  status     - Trang thai server\n";
    cout << "  quit       - Thoat chuong trinh\n\n";
}




//FTPServer ftp(21);  // FTP port 21 mặc định
//FTPServerCLI::FTPServerCLI()
//{
//    serverRunning = false;
//    setCmdPrompt("FTPServer> ");
//    initCmd();   // đăng ký lệnh
//}
//
//// đăng ký các lệnh vào CLI framework
//void FTPServerCLI::initCmd()
//{
//    addCmd("help",  CLI_CAST(&FTPServerCLI::cmdHelp));
//    addCmd("start", CLI_CAST(&FTPServerCLI::cmdStart));
//    addCmd("stop",  CLI_CAST(&FTPServerCLI::cmdStop));
//    addCmd("status",CLI_CAST(&FTPServerCLI::cmdStatus));
//}
//
//// =========================
////  Các lệnh thực thi
//// =========================
//
//// lệnh HELP
//void FTPServerCLI::cmdHelp(string argv[], int argc)
//{
//    cout << "\nDanh sach lenh ho tro:\n";
//    cout << "  help       - Hien thi tro giup\n";
//    cout << "  start      - Khoi dong FTP server\n";
//    cout << "  stop       - Dung server\n";
//    cout << "  status     - Trang thai server\n";
//    cout << "  quit       - Thoat chuong trinh\n\n";
//}
//
//// lệnh START server
//void FTPServerCLI::cmdStart(string argv[], int argc)
//{
//    if (ftp.isRunning())
//        cout << "Server dang chay roi!\n";
//    else {
//        if (ftp.start())
//            cout << ">>> FTP Server dang lang nghe tren cong 21...\n";
//        else
//            cout << "[LOI] Khong the khoi dong FTP Server!\n";
//    }
//}
//
//// lệnh STOP server
//void FTPServerCLI::cmdStop(string argv[], int argc)
//{
//    if (!ftp.isRunning())
//        cout << "Server chua chay!\n";
//    else {
//        ftp.stop();
//        cout << ">>> FTP Server da dung.\n";
//    }
//}
//
//// lệnh STATUS server
//void FTPServerCLI::cmdStatus(string argv[], int argc)
//{
//    cout << "Trang thai server: "
//         << (ftp.isRunning() ? "DANG CHAY" : "DANG TAT") << "\n";
//}
