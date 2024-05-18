#include "RequestProtocolDeserializer.hpp"

RequestProtocolDeserializer::RequestProtocolDeserializer(std::string content) : m_content(content)
{
    p_state = std::make_shared<RequestProtocolDeserializerStartState>();
    m_index = 0;
}

std::optional<Error> RequestProtocolDeserializer::TransmitState(std::shared_ptr<IRequestProtocolDeserializerState> state)
{
    p_state = nullptr;
    return p_state->Execute(m_protocolBuilder, m_stringBuilder, m_content, m_index);
}

std::variant<RequestProtocol, Error> RequestProtocolDeserializer::Deserialize()
{
    auto response = TransmitState(std::make_shared<RequestProtocolDeserializerStartState>());
    if (response.has_value())
        return response.value();

    return m_protocolBuilder.Build();
}

std::optional<Error> RequestProtocolDeserializerStartState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    if (isdigit(content[index]))
    {
        index++;
        stringBuilder << content[index];
        return p_deserializer->TransmitState(std::make_shared<RequestProtocolDeserializerGetVersionState>());
    }
    else
        return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}

std::optional<Error> RequestProtocolDeserializerGetVersionState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (isdigit(content[index]) || content[index] == '.')
    {
        index++;
        stringBuilder << content[index];
    }

    if (content[index] == ';')
    {
        index++;
        builder.AddVersion(stringBuilder.str());
        stringBuilder.str("");
        stringBuilder.clear();
        return p_deserializer->TransmitState(std::make_shared<RequestProtocolDeserializerGetCommandTypeState>());
    }

    return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}

std::optional<Error> RequestProtocolDeserializerGetCommandTypeState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (isdigit(content[index]) || isalpha(content[index]))
    {
        index++;
        stringBuilder << content[index];
    }

    if (content[index] == ';')
    {
        index++;
        std::string commandTypeText = stringBuilder.str();
        if (commandTypeText == "RawQuery" || commandTypeText == "StoredProcedure")
        {
            builder.AddCommandType(commandTypeText == "RawQuery" ? RequestProtocolCommandType::RawQuery : RequestProtocolCommandType::StoredProcedure);
            stringBuilder.str("");
            stringBuilder.clear();
            return p_deserializer->TransmitState(std::make_shared<RequestProtocolDeserializerGetHeaderNameState>());
        }
    }

    return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}

std::optional<Error> RequestProtocolDeserializerGetHeaderNameState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (isdigit(content[index]) || isalpha(content[index]))
    {
        index++;
        stringBuilder << content[index];
    }

    if (content[index] == ':')
    {
        index++;
        builder.AddHeaderName(stringBuilder.str());
        stringBuilder.str("");
        stringBuilder.clear();
        return p_deserializer->TransmitState(std::make_shared<RequestProtocolDeserializerGetHeaderValueState>());
    }
    else if (content[index] == ';' && stringBuilder.str().length() == 0)
    {
        index++;
        return p_deserializer->TransmitState(std::make_shared<RequestProtocolDeserializerGetMessageState>());
    }

    return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}

std::optional<Error> RequestProtocolDeserializerGetHeaderValueState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (isdigit(content[index]) || isalpha(content[index]))
    {
        index++;
        stringBuilder << content[index];
    }

    if (content[index] == ';')
    {
        index++;
        builder.AddHeaderValue(stringBuilder.str());
        stringBuilder.str("");
        stringBuilder.clear();
        return p_deserializer->TransmitState(std::make_shared<RequestProtocolDeserializerGetHeaderNameState>());
    }

    return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}

std::optional<Error> RequestProtocolDeserializerGetMessageState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (isdigit(content[index]) || isalpha(content[index]))
    {
        index++;
        stringBuilder << content[index];
    }

    if (index == content.length() - 1)
    {
        builder.AddMessage(stringBuilder.str());
        stringBuilder.str("");
        stringBuilder.clear();
        return std::nullopt;
    }

    return Error("The version doesn't start with a number", ErrorType::Unexpected); // I need to improve this one
}