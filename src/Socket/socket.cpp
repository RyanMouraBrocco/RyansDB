#include "socket.hpp"

Socket::Socket(u_int16_t port)
{
    m_socketListener = CreateSocketListenerToIpv4();
    if (m_socketListener < 0)
    {
        m_status = SocketStatus::Fail;
        return;
    }

    int bindResult = BindSocketListenerToPort(m_socketListener, port);
    if (bindResult < 0)
    {
        m_status = SocketStatus::Fail;
        return;
    }
}

Socket::~Socket()
{
    if (m_status == SocketStatus::Opened)
        close(m_socketListener);
}

void Socket::Listen()
{
    if (m_status == SocketStatus::Fail)
        return;

    if (listen(m_socketListener, SOMAXCONN) == -1)
    {
        // std::cerr << "Can't listen";
        m_status = SocketStatus::Fail;
    }
    else
        m_status = SocketStatus::Opened;
}

int Socket::GetListener() const
{
    return m_socketListener;
}

SocketStatus Socket::GetStatus() const
{
    return m_status;
}

int Socket::CreateSocketListenerToIpv4()
{
    int socketListener = socket(AF_INET, SOCK_STREAM, 0);
    // if (socketListener == -1)
    // {
    //     std::cerr << "Error to create socket listener";
    //     return -1;
    // }

    return socketListener;
}

int Socket::BindSocketListenerToPort(int socketListener, u_int16_t port)
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(socketListener, (sockaddr *)&hint, sizeof(hint)) == -1)
    {
        // std::cerr << "Can't bind to IP/port";
        return -2;
    }

    return 1;
}