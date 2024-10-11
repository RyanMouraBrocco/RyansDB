#include "socket.hpp"

Socket::Socket(u_int16_t port)
{
    auto socketListenerResult = CreateSocketListenerToIpv4();
    if (std::holds_alternative<Error>(socketListenerResult))
    {
        UpsertError(std::get<Error>(socketListenerResult));
        m_status = SocketStatus::Fail;
        return;
    }

    m_socketListener = std::get<int>(socketListenerResult);

    auto bindResult = BindSocketListenerToPort(m_socketListener, port);
    if (bindResult.has_value())
    {
        UpsertError(bindResult.value());
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
        UpsertError(Error(ErrorType::Unexpected, "Cannot listen the socket port"));
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
        return Error(ErrorType::Unexpected, "Error to create socket listener");
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
        return Error(ErrorType::Unexpected, "Cannot bind the socket listener to IP/port " + std::to_string(port));
    }

    return std::nullopt;
}

void Socket::UpsertError(Error error)
{
    if (p_error == nullptr)
        p_error = std::make_shared<Error>(error);
    else
        *p_error = error;
}

void Socket::Close()
{
    close(m_socketListener);
    m_status = SocketStatus::Closed;
}