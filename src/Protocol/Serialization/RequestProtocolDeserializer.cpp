#include "RequestProtocolDeserializer.hpp"

RequestProtocolDeserializer::RequestProtocolDeserializer(std::string content) : m_content(content)
{
    p_state = std::make_unique<RequestProtocolDeserializerStartState>();
    m_index = 0;
}

std::variant<RequestProtocol, Error> RequestProtocolDeserializer::Deserialize()
{
    p_state->Execute(m_builder, m_content, m_index);
}

std::optional<Error> RequestProtocolDeserializerStartState::Execute(RequestProtocolBuilder &builder, std::string &content, int &index)
{
    if (test::IsNumber(content[index]))
    {
        index++;
        return std::nullopt;
    }
    else
        return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}
