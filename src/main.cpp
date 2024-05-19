// C++ libraries
#include <iostream>
#include "TcpServer/Socket/socket.hpp"
#include "TcpServer/Socket/Connections/connection.hpp"
#include "Protocol/Serialization/RequestProtocolDeserializer.hpp"
#include "Protocol/Contracts/Request/RequestProtocol.hpp"
#include <thread>

void EndPointsListener(Connection *connection)
{
    while (true)
    {
        connection->AcceptForNewAccess();

        char buf[4096];
        while (connection->GetStatus() == ConnectionStatus::Connected)
        {
            auto textResult = connection->ReceiveBytes(4096);
            if (textResult.has_value())
            {
                if (connection->GetStatus() != ConnectionStatus::Connected)
                    break;

                auto deserializer = RequestProtocolDeserializer(textResult.value());
                auto requestProtocolResult = deserializer.Deserialize();
                if (std::holds_alternative<RequestProtocol>(requestProtocolResult))
                {
                    auto protocol = std::get<RequestProtocol>(requestProtocolResult);
                }

                connection->SendBytes(((std::string) "12345678").data(), 8);
            }
            else if (connection->GetStatus() == ConnectionStatus::Fail)
            {
                std::cout << "Connection Fails" << std::endl;
            }
            else
            {
                std::cout << "Client has disconnected" << std::endl;
            }
        }
    }
}

int main()
{
    Socket socket = Socket(5007);
    socket.Listen();
    if (socket.GetStatus() != SocketStatus::Opened)
    {
        return -1;
    }

    short connectionPool = 1;

    Connection **connections = new Connection *[connectionPool];
    std::thread **threads = new std::thread *[connectionPool];

    for (short i = 0; i < connectionPool; i++)
    {
        connections[i] = new Connection(socket);
        threads[i] = new std::thread(EndPointsListener, connections[i]);
    }

    for (short i = 0; i < connectionPool; i++)
    {
        threads[i]->join();
    }

    for (short i = 0; i < connectionPool; i++)
    {
        delete connections[i];
        delete threads[i];
    }

    delete[] connections;
    delete[] threads;

    return 0;
}