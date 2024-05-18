#pragma once

#include "Builders/RequestProtocolBuilder.hpp"
#include "../Contracts/Request/RequestProtocol.hpp"
#include "../../Error/error.hpp"
#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <sstream>
#include <ctype.h>

class RequestProtocolDeserializer;

class IRequestProtocolDeserializerState
{
protected:
    std::unique_ptr<RequestProtocolDeserializer> p_deserializer;

public:
    virtual std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) = 0;
};

class RequestProtocolDeserializer
{
private:
    RequestProtocolBuilder m_protocolBuilder;
    std::stringstream m_stringBuilder;
    std::string m_content;
    int m_index;
    std::shared_ptr<IRequestProtocolDeserializerState> p_state;

public:
    RequestProtocolDeserializer(std::string content);
    std::optional<Error> TransmitState(std::shared_ptr<IRequestProtocolDeserializerState> state);
    std::variant<RequestProtocol, Error> Deserialize();
};

class RequestProtocolDeserializerStartState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetVersionState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetCommandTypeState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetHeaderNameState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetHeaderValueState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetMessageState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::stringstream &stringBuilder, std::string &content, int &index) override;
};