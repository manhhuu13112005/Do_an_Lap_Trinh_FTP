/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002 ...
 *   Updated in 2020
 */

#include "socket.h"

// ===== FIX LỖI MinGW GCC 14: xung đột std::byte vs WinAPI byte =====
// Định nghĩa byte thành tên khác trước khi kéo windows header vào
#ifdef _WIN32
    #define byte win_byte_override
#endif
// ==================================================================

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>

    // Gỡ định nghĩa lại về bình thường
    #undef byte

    typedef int socklen_t;
    typedef char raw_type;
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netinet/in.h>
    typedef void raw_type;
    #include <cstring>
#endif

#include <errno.h>

using namespace std;

#ifdef _WIN32
static bool initialized = false;
#endif

// --------- Phần còn lại CODE GIỮ NGUYÊN như bạn gửi ----------

// SocketException Code
SocketException::SocketException(const string &message, bool inclSysMsg) throw() : userMessage(message)
{
    if (inclSysMsg)
    {
#ifdef _WIN32
        userMessage.append(": ");
        userMessage.append(strerror(WSAGetLastError()));
#else
        userMessage.append(": ");
        userMessage.append(strerror(errno));
#endif
    }
}

SocketException::~SocketException() throw() {}

const char *SocketException::what() const throw()
{
    return userMessage.c_str();
}

void fillAddr(const string &address, unsigned short port, sockaddr_in &addr)
{
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    hostent *host;
    if ((host = gethostbyname(address.c_str())) == NULL)
    {
        throw SocketException("Failed to resolve name (gethostbyname())");
    }
    addr.sin_addr.s_addr = *((unsigned long *)host->h_addr_list[0]);
    addr.sin_port = htons(port);
}

Socket::Socket(int type, int protocol)
{
    keepAlive = false;

#ifdef _WIN32
    if (!initialized)
    {
        WORD wVersionRequested;
        WSADATA wsaData;

        wVersionRequested = MAKEWORD(2, 0);
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            throw SocketException("Unable to load WinSock DLL");

        initialized = true;
    }
#endif

    if ((sockDesc = socket(PF_INET, type, protocol)) < 0)
        throw SocketException("Socket creation failed (socket())", true);
}

Socket::Socket(int sockDesc)
{
    keepAlive = false;
    this->sockDesc = sockDesc;
}

Socket::Socket(const Socket &sock)
{
    sockDesc = sock.sockDesc;
}

void Socket::operator=(const Socket &sock)
{
    sockDesc = sock.sockDesc;
}

Socket::~Socket() {}

void Socket::close()
{
#ifdef _WIN32
    ::closesocket(sockDesc);
#else
    ::close(sockDesc);
#endif
    sockDesc = -1;
}

void Socket::startUp()
{
#ifdef _WIN32
    if (!initialized)
    {
        WORD wVersionRequested;
        WSADATA wsaData;

        wVersionRequested = MAKEWORD(2, 0);
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            throw SocketException("Unable to load WinSock DLL");

        initialized = true;
    }
#endif
}

string Socket::getLocalAddress()
{
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getsockname(sockDesc, (sockaddr *)&addr, (socklen_t *)&addr_len) < 0)
        throw SocketException("Fetch of local address failed (getsockname())", true);

    return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort()
{
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getsockname(sockDesc, (sockaddr *)&addr, (socklen_t *)&addr_len) < 0)
        throw SocketException("Fetch of local port failed", true);

    return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort)
{
    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(localPort);

    if (bind(sockDesc, (sockaddr *)&localAddr, sizeof(sockaddr_in)) < 0)
        throw SocketException("Set of local port failed (bind())", true);
}

void Socket::setLocalAddressAndPort(const string &localAddress, unsigned short localPort)
{
    sockaddr_in localAddr;
    fillAddr(localAddress, localPort, localAddr);

    if (bind(sockDesc, (sockaddr *)&localAddr, sizeof(sockaddr_in)) < 0)
        throw SocketException("Set of local address and port failed (bind())", true);
}

void Socket::cleanUp()
{
#ifdef _WIN32
    if (WSACleanup() != 0)
        throw SocketException("WSACleanup() failed");
#endif
}

unsigned short Socket::resolveService(const string &service, const string &protocol)
{
    servent *serv;
    if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
        return atoi(service.c_str());
    else
        return ntohs(serv->s_port);
}

string Socket::getAddressFromName(const string &name)
{
    hostent *pHost = gethostbyname(name.c_str());
    in_addr addr;
    if (pHost == NULL)
        throw SocketException("Failed to resolve name (gethostbyname())");

    addr.s_addr = *((unsigned long *)pHost->h_addr_list[0]);
    return inet_ntoa(addr);
}

string Socket::getHostname()
{
    char hostname[128];
    int r = gethostname(hostname, 128);
    if (r == -1)
        throw SocketException("Failed to resolve hostname (gethostname())", true);

    return hostname;
}
