#ifndef _FTPSERVER_CLI_H_
#define _FTPSERVER_CLI_H_

#include "cli.h"
#include "EchoTCPServer.h"

class FTPServerCLI : public CmdLineInterface
{
private:
    FTPServer* EchoTcpServer;

public:
    FTPServerCLI();  // constructor
    ~FTPServerCLI();
protected:
    virtual void initCmd(); // override to add commands

private:
    // Các hàm xử lý lệnh
    void cmdHelp(string argv[], int argc);
    void cmdStart(string argv[], int argc);
    void cmdStop(string argv[], int argc);
    void cmdStatus(string argv[], int argc);
};

#endif
