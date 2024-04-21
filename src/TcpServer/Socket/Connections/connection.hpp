#pragma once

#include "../socket.hpp"
#include "../../../Error/error.hpp"
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <string.h>

#include <memory>
#include <optional>

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
    std::shared_ptr<Error> p_error = nullptr;

    void UpsertError(Error error);

public:
    Connection(Socket &socketListener);
    ~Connection();
    void AcceptForNewAccess();
    ConnectionStatus GetStatus() const;
    std::optional<std::string> ReceiveBytes(const int &&size);
    ConnectionStatus SendBytes(char *buffer, const int &&size);
};
