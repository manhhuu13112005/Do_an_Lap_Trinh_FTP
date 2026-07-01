#include <iostream>
#include <string>
#include "cli.h"

using namespace std;

// ----------------------
// Kế thừa CmdLineInterface
// ----------------------
class FTPClientCLI : public CmdLineInterface
{
public:
    FTPClientCLI() : CmdLineInterface("ftp> ") {}  // đặt prompt là ftp>

protected:
    void initCmd() override;
    void initConsole() override;

private:
    // Các hàm xử lý lệnh
    void doHelp(string argv[], int argc);
    void doConnect(string argv[], int argc);
    void doLogin(string argv[], int argc);
    void doUpload(string argv[], int argc);
    void doDownload(string argv[], int argc);
};

// ----------------------
// Cài đặt hàm initCmd()
// ----------------------
void FTPClientCLI::initCmd()
{
    addCmd("help", CLI_CAST(&FTPClientCLI::doHelp));
    addCmd("connect", CLI_CAST(&FTPClientCLI::doConnect));
    addCmd("login", CLI_CAST(&FTPClientCLI::doLogin));
    addCmd("upload", CLI_CAST(&FTPClientCLI::doUpload));
    addCmd("download", CLI_CAST(&FTPClientCLI::doDownload));
    addCmd("quit", CLI_CAST(&FTPClientCLI::doQuit)); // có sẵn trong base class
}

void FTPClientCLI::initConsole()
{
    cout << "==============================" << endl;
    cout << "   FTP Client v3 Interface    " << endl;
    cout << "==============================" << endl;
    cout << "Nhap 'help' de xem cac lenh." << endl;
}

// ----------------------
// Các hàm xử lý lệnh
// ----------------------
void FTPClientCL
