#pragma once

#include "Builders/RequestProtocolBuilder.hpp"
#include "../Contracts/Request/RequestProtocol.hpp"
#include "../../Error/error.hpp"
#include <string>
#include <memory>
#include <variant>
#include <optional>

namespace test
{
    static bool IsNumber(char value)
    {
        return value == '0' ||
               value == '1' ||
               value == '2' ||
               value == '3' ||
               value == '4' ||
               value == '5' ||
               value == '6' ||
               value == '7' ||
               value == '8' ||
               value == '9';
    }

};

class IRequestProtocolDeserializerState
{
public:
    virtual std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) = 0;
};

class RequestProtocolDeserializer
{
private:
    RequestProtocolBuilder m_builder;
    std::string m_content;
    int m_index;
    std::unique_ptr<IRequestProtocolDeserializerState> p_state;

public:
    RequestProtocolDeserializer(std::string content);
    std::variant<RequestProtocol, Error> Deserialize();
};

class RequestProtocolDeserializerStartState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetVersionState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetCommandTypeState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetHeaderNameState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetHeaderValueState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerGetMessageState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};

class RequestProtocolDeserializerErrorState : public IRequestProtocolDeserializerState
{
    std::optional<Error> Execute(RequestProtocolBuilder &builder, std::string &content, int &index) override;
};