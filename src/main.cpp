// C++ libraries
#include <iostream>
#include "TcpServer/Socket/socket.hpp"
#include "TcpServer/Socket/Connections/connection.hpp"
#include "Protocol/Serialization/RequestProtocolDeserializer.hpp"
#include "Protocol/Contracts/Request/RequestProtocol.hpp"
#include "Orchestrator/RequestOrchestrator.hpp"
#include <thread>

#include "StorageEngine/BTree/BTree.hpp"

void EndPointsListener(Connection *connection)
{
    auto orchestrator = RequestOrchestrator();
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

                orchestrator.Consume(textResult.value());

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
    // Socket socket = Socket(5007);
    // socket.Listen();
    // if (socket.GetStatus() != SocketStatus::Opened)
    // {
    //     return -1;
    // }

    // short connectionPool = 1;

    // Connection **connections = new Connection *[connectionPool];
    // std::thread **threads = new std::thread *[connectionPool];

    // for (short i = 0; i < connectionPool; i++)
    // {
    //     connections[i] = new Connection(socket);
    //     threads[i] = new std::thread(EndPointsListener, connections[i]);
    // }

    // for (short i = 0; i < connectionPool; i++)
    // {
    //     threads[i]->join();
    // }

    // for (short i = 0; i < connectionPool; i++)
    // {
    //     delete connections[i];
    //     delete threads[i];
    // }

    // delete[] connections;
    // delete[] threads;

    BTree btree;
    btree.Insert(BTreeKey(1), 10);
    btree.Insert(BTreeKey(2), 10);
    btree.Insert(BTreeKey(3), 10);
    btree.Insert(BTreeKey(4), 10);
    btree.Insert(BTreeKey(5), 10);
    btree.Insert(BTreeKey(6), 10);
    btree.Insert(BTreeKey(7), 10);
    btree.Insert(BTreeKey(8), 10);
    btree.Insert(BTreeKey(9), 10);
    btree.Insert(BTreeKey(10), 10);
    btree.Insert(BTreeKey(11), 10);
    btree.Insert(BTreeKey(12), 10);
    btree.Insert(BTreeKey(13), 10);
    btree.Insert(BTreeKey(14), 10);
    btree.Insert(BTreeKey(15), 10);
    btree.Insert(BTreeKey(16), 10);
    btree.Insert(BTreeKey(17), 10);
    btree.Insert(BTreeKey(18), 10);
    btree.Insert(BTreeKey(19), 10);
    btree.Insert(BTreeKey(20), 10);
    btree.Insert(BTreeKey(21), 10);
    btree.Insert(BTreeKey(22), 10);
    btree.Insert(BTreeKey(23), 10);
    btree.Insert(BTreeKey(24), 10);
    btree.Insert(BTreeKey(25), 10);
    btree.Insert(BTreeKey(26), 10);
    btree.Insert(BTreeKey(27), 10);
    btree.Insert(BTreeKey(28), 10);
    btree.Insert(BTreeKey(29), 10);
    btree.Insert(BTreeKey(30), 10);

    return 0;
}