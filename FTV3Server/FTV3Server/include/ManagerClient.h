#ifndef MANAGERCLIENT_H_
#define MANAGERCLIENT_H_

#include "serverconfig.h"
#include "session.h"

// ManagerClient kế thừa Account, ServerConfig và Session
class ManagerClient : public Account, public ServerConfig, public Session
{
private:
    bool loggedIn; // Trạng thái đăng nhập của client
    string currentSessionID; // ID của phiên đăng nhập

public:
    ManagerClient(const TcpSocket& slave, ServerConfig* conf, const string& username);
    virtual ~ManagerClient();

    // Hàm đăng nhập của client
    bool login(const string& password);

    // Hàm đăng xuất của client
    void logout();

    // Kiểm tra client có đăng nhập hay không
    bool isLoggedIn() const;

    // Hàm lấy Session ID của phiên đăng nhập
    string getSessionID() const;

    // Ghi lại thông tin phiên làm việc
    void saveSessionInfo();

    // Kế thừa từ Session: doUnknown
    void doUnknown(string cmd_argv[], int cmd_argc) override;

    // Kế thừa từ Session: reset
    void reset() override;
};

#endif // MANAGERCLIENT_H_
