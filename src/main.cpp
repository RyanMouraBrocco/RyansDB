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
    btree.Insert(BTreeKey(31), 10);
    btree.Insert(BTreeKey(32), 10);
    btree.Insert(BTreeKey(33), 10);
    btree.Insert(BTreeKey(34), 10);
    btree.Insert(BTreeKey(35), 10);
    btree.Insert(BTreeKey(36), 10);
    btree.Insert(BTreeKey(37), 10);
    btree.Insert(BTreeKey(38), 10);
    btree.Insert(BTreeKey(39), 10);
    btree.Insert(BTreeKey(40), 10);
    btree.Insert(BTreeKey(41), 10);
    btree.Insert(BTreeKey(42), 10);
    btree.Insert(BTreeKey(43), 10);
    btree.Insert(BTreeKey(44), 10);
    btree.Insert(BTreeKey(45), 10);
    btree.Insert(BTreeKey(46), 10);
    btree.Insert(BTreeKey(47), 10);
    btree.Insert(BTreeKey(48), 10);
    btree.Insert(BTreeKey(49), 10);
    btree.Insert(BTreeKey(50), 10);
    btree.Insert(BTreeKey(51), 10);
    btree.Insert(BTreeKey(52), 10);
    btree.Insert(BTreeKey(53), 10);
    btree.Insert(BTreeKey(54), 10);
    btree.Insert(BTreeKey(55), 10);
    btree.Insert(BTreeKey(56), 10);
    btree.Insert(BTreeKey(57), 10);
    btree.Insert(BTreeKey(58), 10);
    btree.Insert(BTreeKey(59), 10);
    btree.Insert(BTreeKey(60), 10);
    btree.Insert(BTreeKey(61), 10);
    btree.Insert(BTreeKey(62), 10);
    btree.Insert(BTreeKey(63), 10);
    btree.Insert(BTreeKey(64), 10);
    btree.Insert(BTreeKey(65), 10);
    btree.Insert(BTreeKey(66), 10);
    btree.Insert(BTreeKey(67), 10);
    btree.Insert(BTreeKey(68), 10);
    btree.Insert(BTreeKey(69), 10);
    btree.Insert(BTreeKey(70), 10);
    btree.Insert(BTreeKey(71), 10);
    btree.Insert(BTreeKey(72), 10);
    btree.Insert(BTreeKey(73), 10);
    btree.Insert(BTreeKey(74), 10);
    btree.Insert(BTreeKey(75), 10);
    btree.Insert(BTreeKey(76), 10);
    btree.Insert(BTreeKey(77), 10);
    btree.Insert(BTreeKey(78), 10);
    btree.Insert(BTreeKey(79), 10);
    btree.Insert(BTreeKey(80), 10);
    btree.Insert(BTreeKey(81), 10);
    btree.Insert(BTreeKey(82), 10);
    btree.Insert(BTreeKey(83), 10);
    btree.Insert(BTreeKey(84), 10);
    btree.Insert(BTreeKey(85), 10);
    btree.Insert(BTreeKey(86), 10);
    btree.Insert(BTreeKey(87), 10);
    btree.Insert(BTreeKey(88), 10);
    btree.Insert(BTreeKey(89), 10);
    btree.Insert(BTreeKey(90), 10);
    btree.Insert(BTreeKey(91), 10);
    btree.Insert(BTreeKey(92), 10);
    btree.Insert(BTreeKey(93), 10);
    btree.Insert(BTreeKey(94), 10);
    btree.Insert(BTreeKey(95), 10);
    btree.Insert(BTreeKey(96), 10);
    btree.Insert(BTreeKey(97), 10);
    btree.Insert(BTreeKey(98), 10);
    btree.Insert(BTreeKey(99), 10);
    btree.Insert(BTreeKey(100), 10);
    btree.Insert(BTreeKey(110), 10);
    btree.Insert(BTreeKey(111), 10);
    btree.Insert(BTreeKey(112), 10);
    btree.Insert(BTreeKey(113), 10);
    btree.Insert(BTreeKey(114), 10);
    btree.Insert(BTreeKey(115), 10);
    btree.Insert(BTreeKey(116), 10);
    btree.Insert(BTreeKey(117), 10);
    btree.Insert(BTreeKey(118), 10);
    btree.Insert(BTreeKey(119), 10);
    btree.Insert(BTreeKey(120), 10);

    return 0;
}