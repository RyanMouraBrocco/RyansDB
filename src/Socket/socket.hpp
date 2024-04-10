#pragma once

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

enum class SocketStatus
{
    Closed,
    Opened,
    Fail
};

class Socket
{
private:
    int m_socketListener;
    SocketStatus m_status;

    int CreateSocketListenerToIpv4();
    int BindSocketListenerToPort(int socketListener, u_int16_t port);

public:
    Socket(u_int16_t port);
    ~Socket();
    void Listen();
    int GetListener() const;
    SocketStatus GetStatus() const;
};
