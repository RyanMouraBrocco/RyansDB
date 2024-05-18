#include "connection.hpp"

Connection::Connection(Socket &socketListener) : m_socketListener(socketListener) {}

Connection::~Connection()
{
    if (m_status != ConnectionStatus::Closed)
        close(m_clientSocket);
}

void Connection::AcceptForNewAccess()
{
    m_clientSocket = accept(m_socketListener.GetListener(), (sockaddr *)&m_client, &m_clientSize);
    if (m_clientSocket == -1)
        m_status = ConnectionStatus::Fail;
    else
        m_status = ConnectionStatus::Connected;
}

ConnectionStatus Connection::GetStatus() const
{
    return m_status;
}

std::optional<std::string> Connection::ReceiveBytes(const int &&size)
{
    char buffer[size];
    memset(buffer, 0, size);
    int bytesRecv = recv(m_clientSocket, buffer, size, 0);

    if (bytesRecv == -1)
    {
        UpsertError(Error(ErrorType::Unexpected, "There was a connection issue"));
        m_status = ConnectionStatus::Fail;
        return std::nullopt;
    }

    if (bytesRecv == 0)
    {
        UpsertError(Error(ErrorType::Unexpected, "The client disconnected"));
        m_status = ConnectionStatus::Closed;
        return std::nullopt;
    }

    return std::string(buffer, 0, bytesRecv);
}

ConnectionStatus Connection::SendBytes(char *buffer, const int &&size)
{
    int result = send(m_clientSocket, buffer, size + 1, 0);
    if (result == -1)
        m_status = ConnectionStatus::Fail;

    return m_status;
}

void Connection::UpsertError(Error error)
{
    if (p_error == nullptr)
        p_error = std::make_shared<Error>(error);
    else
        *p_error = error;
}