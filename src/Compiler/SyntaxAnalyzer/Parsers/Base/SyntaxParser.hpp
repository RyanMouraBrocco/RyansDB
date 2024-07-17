#pragma once

#include <optional>
#include <memory>
#include "../../../../Error/error.hpp"
#include "../../../SymbolTable/SymbolTable.hpp"

class SyntaxParser
{
private:
    std::optional<Error> CheckIfCurrentTokenInValid(const std::vector<TokenDefinition> &tokens, const Token &expextedToken, const int &index) const
    {
        if (index >= tokens.size())
            return Error(ErrorType::InvalidToken, "The command finished but it was expected a " + tokens[index].GetUpperCaseLexeme());

        if (tokens[index].GetToken() != expextedToken)
            return Error(ErrorType::InvalidToken, "It was expected a <HERE> but was found a " + tokens[index].GetUpperCaseLexeme());

        return std::nullopt;
    }

protected:
    SyntaxParser(){};
    ~SyntaxParser(){};

    std::optional<Error> Consume(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, const Token &&expextedToken, int &index) const
    {
        std::optional<Error> validTokenResult = CheckIfCurrentTokenInValid(tokens, expextedToken, index);
        if (validTokenResult.has_value())
            return validTokenResult;

        index++;

        return std::nullopt;
    }

    std::optional<Error> AddInAbstractSyntaxTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, const Token &&expextedToken, int &index) const
    {
        std::optional<Error> validTokenResult = CheckIfCurrentTokenInValid(tokens, expextedToken, index);
        if (validTokenResult.has_value())
            return validTokenResult;

        symbolTable->AddNode(tokens[index]);

        index++;

        return std::nullopt;
    }

public:
    SyntaxParser(SyntaxParser &other) = delete;
    SyntaxParser(SyntaxParser &&other) = delete;
    void operator=(const SyntaxParser &) = delete;
};