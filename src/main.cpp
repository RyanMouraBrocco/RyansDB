// C++ libraries
#include <iostream>
#include "TcpServer/Socket/socket.hpp"
#include "TcpServer/Socket/Connections/connection.hpp"
#include "Protocol/Serialization/RequestProtocolDeserializer.hpp"
#include "Protocol/Contracts/Request/RequestProtocol.hpp"
#include "Orchestrator/RequestOrchestrator.hpp"
#include <thread>

#include "StorageEngine/BTree/BTree.hpp"
#include <chrono>

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
    // btree.Insert(BTreeKey(1), 10);
    // btree.Insert(BTreeKey(16), 10);
    // btree.Insert(BTreeKey(24), 10);
    // btree.Insert(BTreeKey(4), 10);
    // btree.Insert(BTreeKey(41), 10);
    // btree.Insert(BTreeKey(17), 10);
    // btree.Insert(BTreeKey(5), 10);
    // btree.Insert(BTreeKey(8), 10);
    // btree.Insert(BTreeKey(23), 10);
    // btree.Insert(BTreeKey(9), 10);
    // btree.Insert(BTreeKey(11), 10);
    // btree.Insert(BTreeKey(234), 10);
    // btree.Insert(BTreeKey(6), 10);
    // btree.Insert(BTreeKey(3), 10);
    // btree.Insert(BTreeKey(13), 10);
    // btree.Insert(BTreeKey(38), 10);
    // btree.Insert(BTreeKey(15), 10);
    // btree.Insert(BTreeKey(10), 10);
    // btree.Insert(BTreeKey(77), 10);
    // btree.Insert(BTreeKey(15), 10);
    // btree.Insert(BTreeKey(28), 10);

    // auto value = btree.FindOne(BTreeKey(22));

    // delete test 1
    // btree.Insert(BTreeKey(1), 10);
    // btree.Insert(BTreeKey(2), 10);
    // btree.Insert(BTreeKey(3), 10);

    // btree.Delete(BTreeKey(2));
    // btree.Delete(BTreeKey(1));
    // btree.Delete(BTreeKey(3));

    // delete test 2
    // btree.Insert(BTreeKey(1), 10);
    // btree.Insert(BTreeKey(16), 10);
    // btree.Insert(BTreeKey(24), 10);
    // btree.Insert(BTreeKey(4), 10);
    // btree.Insert(BTreeKey(41), 10);
    // btree.Insert(BTreeKey(17), 10);
    // btree.Insert(BTreeKey(5), 10);
    // btree.Insert(BTreeKey(8), 10);
    // btree.Insert(BTreeKey(23), 10);
    // btree.Insert(BTreeKey(9), 10);
    // btree.Insert(BTreeKey(11), 10);
    // btree.Insert(BTreeKey(234), 10);
    // btree.Insert(BTreeKey(6), 10);
    // btree.Insert(BTreeKey(3), 10);
    // btree.Insert(BTreeKey(13), 10);
    // btree.Insert(BTreeKey(38), 10);
    // btree.Insert(BTreeKey(15), 10);
    // btree.Insert(BTreeKey(10), 10);
    // btree.Insert(BTreeKey(77), 10);
    // btree.Insert(BTreeKey(15), 10);
    // btree.Insert(BTreeKey(28), 10);

    // btree.Delete(BTreeKey(11));
    // btree.Delete(BTreeKey(9));
    // btree.Delete(BTreeKey(10));

    // auto all = btree.ListAll();

    // delete test 3
    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BTreeKey(i), 10);
    // }

    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Delete(BTreeKey(i));
    // }

    // delete test 4
    for (int i = 0; i < 100; i++)
    {
        btree.Insert(BTreeKey(i), 10);
    }

    for (int i = 99; i >= 0; i--)
    {
        std::cout << i << std::endl;
        if (i == 71)
        {
            int a = 2;
            a = 3;
        }
        btree.Delete(BTreeKey(i));
    }

    return 0;
}