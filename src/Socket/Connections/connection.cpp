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

std::string Connection::ReceiveBytes(const int &&size)
{
    char buffer[size];
    memset(buffer, 0, size);
    int bytesRecv = recv(m_clientSocket, buffer, size, 0);

    if (bytesRecv == -1)
    {
        // std::cerr << "There was a connection issue" << std::endl;
        m_status = ConnectionStatus::Fail;
        return nullptr;
    }

    if (bytesRecv == 0)
    {
        // std::cout << "The client disconnected" << std::endl;
        m_status = ConnectionStatus::Closed;
        return nullptr;
    }

    return std::string(buffer, 0, bytesRecv);
}

void Connection::SendBytes(char *buffer, const int &&size)
{
    int result = send(m_clientSocket, buffer, size + 1, 0);
    if (result == -1)
        m_status = ConnectionStatus::Fail;
}