// C++ libraries
#include <iostream>
#include "Socket/socket.hpp"
#include "Socket/Connections/connection.hpp"
#include <thread>

void EndPointsListener(Connection *connection)
{
    connection->AcceptForNewAccess();

    char buf[4096];
    while (true)
    {
        std::string text = connection->ReceiveBytes(4096);

        if (connection->GetStatus() != ConnectionStatus::Connected)
            break;

        connection->SendBytes("12345678", 8);
    }
}

int main()
{
    Socket socket = Socket(5432);
    socket.Listen();
    if (socket.GetStatus() != SocketStatus::Opened)
    {
        return -1;
    }

    short connectionPool = 40;

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