#include "RequestOrchestrator.hpp"

RequestOrchestrator::RequestOrchestrator() {}

void RequestOrchestrator::Consume(std::string requestMessage)
{
    auto deserializer = RequestProtocolDeserializer(requestMessage);
    auto requestProtocolResult = deserializer.Deserialize();
    if (std::holds_alternative<RequestProtocol>(requestProtocolResult))
    {
        auto protocol = std::get<RequestProtocol>(requestProtocolResult);
        auto compiler = Compiler();
        auto analysParserTreeResult = compiler.Build(protocol.GetMessage());
        auto analysPartserTree = std::get<std::shared_ptr<ParserTreeNode>>(analysParserTreeResult);
        auto executer = ExecuteEngine();
        auto result = executer.Execute(analysPartserTree);
    }
}