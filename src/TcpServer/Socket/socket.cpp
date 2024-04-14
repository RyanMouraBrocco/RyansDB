#include "socket.hpp"

Socket::Socket(u_int16_t port)
{
    auto socketListenerResult = CreateSocketListenerToIpv4();
    if (std::holds_alternative<Error>(socketListenerResult))
    {
        m_error = std::get<Error>(socketListenerResult);
        m_status = SocketStatus::Fail;
        return;
    }

    m_socketListener = std::get<int>(socketListenerResult);

    auto bindResult = BindSocketListenerToPort(m_socketListener, port);
    if (bindResult.has_value())
    {
        m_error = bindResult.value();
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
        m_error = Error("Cannot listen", ErrorType::Unexpected);
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

std::variant<int, Error> Socket::CreateSocketListenerToIpv4()
{
    int socketListener = socket(AF_INET, SOCK_STREAM, 0);
    if (socketListener == -1)
    {
        return Error("Error to create socket listener", ErrorType::Unexpected);
    }

    return socketListener;
}

std::optional<Error> Socket::BindSocketListenerToPort(int socketListener, u_int16_t port)
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(socketListener, (sockaddr *)&hint, sizeof(hint)) == -1)
    {
        return Error("Can't bind to IP/port", ErrorType::Unexpected);
    }

    return std::nullopt;
}