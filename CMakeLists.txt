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
                ./src/Protocol/Serialization/Builders/RequestProtocolBuilder.cpp
                ./src/Orchestrator/RequestOrchestrator.cpp
                ./src/Compiler/Compiler.cpp
                ./src/Compiler/SymbolTable/TokenDefinition/TokenDefinition.cpp
                ./src/Compiler/SymbolTable/ParserTree/ParserTreeNode.cpp
                ./src/Compiler/SymbolTable/ParserTree/Builder/ParserTreeBuilder.cpp
                ./src/Compiler/SymbolTable/SymbolTable.cpp
                ./src/Compiler/LexycalAnalyzer/LexycalAnalyzer.cpp
                ./src/Compiler/SyntaxAnalyzer/SyntaxAnalyzer.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/AlterTableParser/AlterTableParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/CreateDatabaseParser/CreateDatabaseParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/CreateTableParser/CreateTableParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/DeleteParser/DeleteParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/DropDatabaseParser/DropDatabaseParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/DropTableParser/DropTableParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/InsertParser/InsertParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/LogicalOperationParser/LogicalOperationParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/SelectParser/SelectParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/UpdateParser/UpdateParser.cpp
                ./src/Compiler/SyntaxAnalyzer/Parsers/Utils/UtilsParser.cpp
                ./src/Compiler/SemanticAnalyzer/SemanticAnalyzer.cpp
                ./src/ExecutePlan/ExecuteEngine.cpp
                ./src/ExecutePlan/Processors/DatabaseProcessor/DatabaseProcessor.cpp
                ./src/ExecutePlan/Processors/TableProcessor/TableProcessor.cpp
                ./src/ExecutePlan/Processors/TableProcessor/CreateTableProcessor/CreateTableProcessor.cpp
                ./src/StorageEngine/DataAccess.cpp
                ./src/StorageEngine/Repositories/DatabaseRepository/DatabaseRepository.cpp
                ./src/StorageEngine/Contracts/Database/DatabaseDefinition.cpp
                ./src/StorageEngine/Contracts/Pages/MappingPage/MappingPage.cpp
                ./src/StorageEngine/Contracts/Pages/PageFreeSpacePage/PageFreeSpacePage.cpp
                ./src/StorageEngine/Contracts/Pages/TableMappingPage/TableMappingPage.cpp
                ./src/StorageEngine/Contracts/Pages/DataPage/DataPage.cpp)
                