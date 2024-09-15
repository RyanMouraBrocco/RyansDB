// C++ libraries
#include <iostream>
#include "TcpServer/Socket/socket.hpp"
#include "TcpServer/Socket/Connections/connection.hpp"
#include "Protocol/Serialization/RequestProtocolDeserializer.hpp"
#include "Protocol/Contracts/Request/RequestProtocol.hpp"
#include "Orchestrator/RequestOrchestrator.hpp"
#include <thread>

#include "StorageEngine/BPlusTree/BPlusTree.hpp"
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

    // BPlusTree<int> btree;
    // btree.Insert(BPlusTreeKey<int>(1), 10);
    // btree.Insert(BPlusTreeKey(16), 10);
    // btree.Insert(BPlusTreeKey(24), 10);
    // btree.Insert(BPlusTreeKey(4), 10);
    // btree.Insert(BPlusTreeKey(41), 10);
    // btree.Insert(BPlusTreeKey(17), 10);
    // btree.Insert(BPlusTreeKey(5), 10);
    // btree.Insert(BPlusTreeKey(8), 10);
    // btree.Insert(BPlusTreeKey(23), 10);
    // btree.Insert(BPlusTreeKey(9), 10);
    // btree.Insert(BPlusTreeKey(11), 10);
    // btree.Insert(BPlusTreeKey(234), 10);
    // btree.Insert(BPlusTreeKey(6), 10);
    // btree.Insert(BPlusTreeKey(3), 10);
    // btree.Insert(BPlusTreeKey(13), 10);
    // btree.Insert(BPlusTreeKey(38), 10);
    // btree.Insert(BPlusTreeKey(15), 10);
    // btree.Insert(BPlusTreeKey(10), 10);
    // btree.Insert(BPlusTreeKey(77), 10);
    // btree.Insert(BPlusTreeKey(15), 10);
    // btree.Insert(BPlusTreeKey(28), 10);

    // auto value = btree.FindOne(BPlusTreeKey(22));

    // delete test 1
    // btree.Insert(BPlusTreeKey(1), 10);
    // btree.Insert(BPlusTreeKey(2), 10);
    // btree.Insert(BPlusTreeKey(3), 10);

    // btree.Delete(BPlusTreeKey(2));
    // btree.Delete(BPlusTreeKey(1));
    // btree.Delete(BPlusTreeKey(3));

    // delete test 2
    // btree.Insert(BPlusTreeKey(1), 10);
    // btree.Insert(BPlusTreeKey(16), 10);
    // btree.Insert(BPlusTreeKey(24), 10);
    // btree.Insert(BPlusTreeKey(4), 10);
    // btree.Insert(BPlusTreeKey(41), 10);
    // btree.Insert(BPlusTreeKey(17), 10);
    // btree.Insert(BPlusTreeKey(5), 10);
    // btree.Insert(BPlusTreeKey(8), 10);
    // btree.Insert(BPlusTreeKey(23), 10);
    // btree.Insert(BPlusTreeKey(9), 10);
    // btree.Insert(BPlusTreeKey(11), 10);
    // btree.Insert(BPlusTreeKey(234), 10);
    // btree.Insert(BPlusTreeKey(6), 10);
    // btree.Insert(BPlusTreeKey(3), 10);
    // btree.Insert(BPlusTreeKey(13), 10);
    // btree.Insert(BPlusTreeKey(38), 10);
    // btree.Insert(BPlusTreeKey(15), 10);
    // btree.Insert(BPlusTreeKey(10), 10);
    // btree.Insert(BPlusTreeKey(77), 10);
    // btree.Insert(BPlusTreeKey(15), 10);
    // btree.Insert(BPlusTreeKey(28), 10);

    // btree.Delete(BPlusTreeKey(11));
    // btree.Delete(BPlusTreeKey(9));
    // btree.Delete(BPlusTreeKey(10));

    // auto all = btree.ListAll();

    // delete test 3
    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // delete test 4
    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 999'999; i >= 0; i--)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // delete test 5
    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // delete test 6
    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 999'999; i >= 500'000; i--)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // for (int i = 0; i < 500'000; i++)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // delete test 7
    // for (int i = 0; i < 10'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 0; i < 10'000'000; i++)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // for (int i = 9'999'999; i >= 5'000'000; i--)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // for (int i = 0; i < 5'000'000; i++)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // delete test 8
    // for (int i = 0; i < 100; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 0; i < 50; i++)
    // {
    //     auto response = btree.Delete(BPlusTreeKey(i));
    //     if (response.has_value())
    //         std::cout << "NOT FOUND " << i << std::endl;
    // }

    // auto all = btree.ListAll();

    // for (int i = 99; i >= 50; i--)
    // {
    //     btree.Delete(BPlusTreeKey(i));
    // }

    // all = btree.ListAll();

    // delete test 9
    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey<int>(i), 10);
    // }

    // for (int i = 0; i < 500'000; i++)
    // {
    //     auto response = btree.Delete(BPlusTreeKey<int>(i));
    //     if (response.has_value())
    //         std::cout << "NOT FOUND " << i << std::endl;
    // }

    // for (int i = 999'999; i >= 0; i--)
    // {
    //     btree.Delete(BPlusTreeKey<int>(i));
    // }

    // for (int i = 0; i < 1'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey<int>(i), 10);
    // }

    // for (int i = 0; i < 500'000; i++)
    // {
    //     auto response = btree.Delete(BPlusTreeKey<int>(i));
    //     if (response.has_value())
    //         std::cout << "NOT FOUND " << i << std::endl;
    // }

    // for (int i = 999'999; i >= 0; i--)
    // {
    //     btree.Delete(BPlusTreeKey<int>(i));
    // }

    // memory test
    // for (int i = 0; i < 100'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 0; i < 100'000'000; i++)
    // {
    //     auto response = btree.Delete(BPlusTreeKey(i));
    //     if (response.has_value())
    //         std::cout << "NOT FOUND " << i << std::endl;
    // }

    // for (int i = 0; i < 100'000'000; i++)
    // {
    //     btree.Insert(BPlusTreeKey(i), 10);
    // }

    // for (int i = 0; i < 100'000'000; i++)
    // {
    //     auto response = btree.Delete(BPlusTreeKey(i));
    //     if (response.has_value())
    //         std::cout << "NOT FOUND " << i << std::endl;
    // }

    return 0;
}