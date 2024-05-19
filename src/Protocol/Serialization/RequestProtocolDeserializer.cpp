#include "RequestProtocolDeserializer.hpp"

RequestProtocolDeserializer::RequestProtocolDeserializer(std::string content) : m_content(content)
{
    p_state = nullptr;
    m_index = 0;
}

std::optional<Error> RequestProtocolDeserializer::TransmitState(IRequestProtocolDeserializerState *state)
{
    state->SetDeserializer(this);
    p_state.reset(state);
    return p_state->Execute(m_protocolBuilder, m_stringBuilder, m_content, m_index);
}

std::variant<RequestProtocol, Error> RequestProtocolDeserializer::Deserialize()
{
    auto response = TransmitState(new RequestProtocolDeserializerStartState());
    if (response.has_value())
        return response.value();

    return m_protocolBuilder.Build();
}

bool IRequestProtocolDeserializerState::IsValidIndexInContent(std::string &content, int &index) const
{
    return index < content.length();
}

std::optional<Error> RequestProtocolDeserializerStartState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    if (IsValidIndexInContent(content, index) && isdigit(content[index]))
    {
        stringBuilder << content[index];
        index++;
        return p_deserializer->TransmitState(new RequestProtocolDeserializerGetVersionState());
    }

    return Error(ErrorType::ProtocolFormat, "The version doesn't start with a number");
}

std::optional<Error> RequestProtocolDeserializerGetVersionState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (IsValidIndexInContent(content, index) && (isdigit(content[index]) || content[index] == '.'))
    {
        stringBuilder << content[index];
        index++;
    }

    if (!IsValidIndexInContent(content, index))
        return Error(ErrorType::ProtocolFormat, "The content finished before complete the version");

    if (content[index] != ';')
        return Error(ErrorType::ProtocolFormat, "Invalid version format or transition");

    index++;
    builder.AddVersion(stringBuilder.str());
    stringBuilder.str("");
    stringBuilder.clear();
    return p_deserializer->TransmitState(new RequestProtocolDeserializerGetCommandTypeState());
}

std::optional<Error> RequestProtocolDeserializerGetCommandTypeState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (IsValidIndexInContent(content, index) && (isdigit(content[index]) || isalpha(content[index])))
    {
        stringBuilder << content[index];
        index++;
    }

    if (!IsValidIndexInContent(content, index))
        return Error(ErrorType::ProtocolFormat, "The content finished before complete the commandtype");

    if (content[index] != ';')
        return Error(ErrorType::ProtocolFormat, "Invalid commandtype format or transition");

    index++;
    std::string commandTypeText = stringBuilder.str();
    if (commandTypeText != "RawQuery" && commandTypeText != "StoredProcedure")
        return Error(ErrorType::ProtocolFormat, "CommandType " + commandTypeText + " is not allowed");

    builder.AddCommandType(commandTypeText == "RawQuery" ? RequestProtocolCommandType::RawQuery : RequestProtocolCommandType::StoredProcedure);
    stringBuilder.str("");
    stringBuilder.clear();
    return p_deserializer->TransmitState(new RequestProtocolDeserializerGetHeaderNameState());
}

std::optional<Error> RequestProtocolDeserializerGetHeaderNameState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (IsValidIndexInContent(content, index) && (isdigit(content[index]) || isalpha(content[index])))
    {
        stringBuilder << content[index];
        index++;
    }

    if (!IsValidIndexInContent(content, index))
        return Error(ErrorType::ProtocolFormat, "The content finished before complete the headers");

    std::string currentStringBuilderValue = stringBuilder.str();
    int currentStringBuilderLength = currentStringBuilderValue.length();
    if (content[index] == ':' && currentStringBuilderLength > 0)
    {
        index++;
        builder.AddHeaderName(currentStringBuilderValue);
        stringBuilder.str("");
        stringBuilder.clear();
        return p_deserializer->TransmitState(new RequestProtocolDeserializerGetHeaderValueState());
    }
    else if (content[index] == ';' && currentStringBuilderLength == 0)
    {
        index++;
        return p_deserializer->TransmitState(new RequestProtocolDeserializerGetMessageState());
    }

    return Error(ErrorType::ProtocolFormat, "Invalid headers format or transition");
}

std::optional<Error> RequestProtocolDeserializerGetHeaderValueState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (IsValidIndexInContent(content, index) && (isdigit(content[index]) || isalpha(content[index])))
    {
        stringBuilder << content[index];
        index++;
    }

    if (!IsValidIndexInContent(content, index))
        return Error(ErrorType::ProtocolFormat, "The content finished before complete the headers");

    if (content[index] != ';')
        return Error(ErrorType::ProtocolFormat, "Invalid headers format or transition");

    index++;
    builder.AddHeaderValue(stringBuilder.str());
    stringBuilder.str("");
    stringBuilder.clear();
    return p_deserializer->TransmitState(new RequestProtocolDeserializerGetHeaderNameState());
}

std::optional<Error> RequestProtocolDeserializerGetMessageState::Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index)
{
    while (IsValidIndexInContent(content, index) && (isdigit(content[index]) || isalpha(content[index]) || content[index] == ' ' || content[index] == '*' || content[index] == '='))
    {
        stringBuilder << content[index];
        index++;
    }

    if (index < content.length())
        return Error(ErrorType::ProtocolFormat, "Invalid message");

    builder.AddMessage(stringBuilder.str());
    stringBuilder.str("");
    stringBuilder.clear();
    return std::nullopt;
}