#ifndef _ECHO_FTP_CLI_H_
#define _ECHO_FTP_CLI_H_

#include "cli.h"
#include "EchoTcpClient.h"
#include <string>

using namespace std;

class FTPCLI : public CmdLineInterface
{
private:
    EchoTcpClient client;     // Đối tượng client TCP để giao tiếp server

public:
    FTPCLI();
    ~FTPCLI();

    void initCmd();

    // Các lệnh FTP
    void cmdLogin(string cmd_argv[], int cmd_argc);
    void cmdSubmit(string cmd_argv[], int cmd_argc);
    void cmdQuit(string cmd_argv[], int cmd_argc);
    void cmdHelp(string cmd_argv[], int cmd_argc);
};

#endif
