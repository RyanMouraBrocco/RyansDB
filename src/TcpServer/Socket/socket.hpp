#pragma once

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../../Error/error.hpp"

#include <variant>
#include <optional>

#include <memory>

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
    std::shared_ptr<Error> p_error = nullptr;

    void UpsertError(Error error);
    std::variant<int, Error> CreateSocketListenerToIpv4();
    std::optional<Error> BindSocketListenerToPort(int socketListener, u_int16_t port);

public:
    Socket(u_int16_t port);
    ~Socket();
    void Listen();
    int GetListener() const;
    SocketStatus GetStatus() const;
};
