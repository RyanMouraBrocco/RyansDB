#pragma once

#include "../Socket/socket.hpp"
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <string.h>

enum class ConnectionStatus
{
    Closed,
    Connected,
    Fail
};

class Connection
{
private:
    Socket &m_socketListener;
    sockaddr_in m_client;
    socklen_t m_clientSize = sizeof(m_client);
    int m_clientSocket;
    ConnectionStatus m_status = ConnectionStatus::Closed;

public:
    Connection(Socket &socketListener);
    ~Connection();
    void AcceptForNewAccess();
    ConnectionStatus GetStatus() const;
    std::string ReceiveBytes(const int &&size);
    void SendBytes(char *buffer, const int &&size);
};
