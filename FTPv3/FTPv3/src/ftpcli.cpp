#include "ftpcli.h"

// Hàm tạo
FTPCLI::FTPCLI()
{
    setCmdPrompt("FTPv3> "); // Tùy chỉnh dấu nhắc lệnh
    initCmd();               // Gọi hàm đăng ký các lệnh
}

// Hàm hủy
FTPCLI::~FTPCLI()
{
    cout << "Thoát giao diện FTP CLI.\n";
}

// Đăng ký lệnh
void FTPCLI::initCmd()
{
    addCmd("help", CLI_CAST(&FTPCLI::cmdHelp));
    addCmd("connect", CLI_CAST(&FTPCLI::cmdConnect));
    addCmd("upload", CLI_CAST(&FTPCLI::cmdUpload));
}

// Lệnh HELP
void FTPCLI::cmdHelp(string argv[], int argc)
{
    cout << "==== DANH SÁCH LỆNH ====\n";
    cout << "help                - Hiển thị trợ giúp\n";
    cout << "connect <ip> <port> - Kết nối tới server FTP\n";
    cout << "upload <file>       - Upload file lên server\n";
    cout << "quit                - Thoát chương trình\n";
}

// Lệnh CONNECT
void FTPCLI::cmdConnect(string argv[], int argc)
{
    if (argc < 3)
    {
        cout << "Sai cú pháp! Dùng: connect <ip> <port>\n";
        return;
    }
    cout << "Đang kết nối tới server FTP tại " << argv[1] << ":" << argv[2] << "...\n";
    // Ở đây bạn có thể viết thêm phần socket connect thật sự.
}

// Lệnh UPLOAD
void FTPCLI::cmdUpload(string argv[], int argc)
{
    if (argc < 2)
    {
        cout << "Sai cú pháp! Dùng: upload <filename>\n";
        return;
    }
    cout << "Đang upload file: " << argv[1] << "...\n";
    // Ở đây bạn có thể viết thêm phần upload thực tế.
}
