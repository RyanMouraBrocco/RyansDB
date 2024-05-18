cmake_minimum_required(VERSION 3.10)

project(RyansDB VERSION 0.1.0 LANGUAGES C CXX)

file(MAKE_DIRECTORY build/app)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ./app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(RyansDBServer 
                ./src/main.cpp
                ./src/Error/error.cpp
                ./src/TcpServer/Socket/socket.cpp
                ./src/TcpServer/Socket/Connections/connection.cpp
                ./src/Protocol/Contracts/Request/RequestProtocol.cpp
                ./src/Protocol/Contracts/Response/ResponseProtocol.cpp
                ./src/Protocol/Serialization/RequestProtocolDeserializer.cpp
                ./src/Protocol/Serialization/Builders/RequestProtocolBuilder.cpp)