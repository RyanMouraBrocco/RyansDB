#pragma once

#include <optional>
#include <memory>
#include "../../../../Error/error.hpp"
#include "../../../SymbolTable/SymbolTable.hpp"

class SyntaxParser
{
protected:
    SyntaxParser(){};
    ~SyntaxParser(){};

    std::optional<Error> Consume(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, const Token &&expextedToken, int &index) const
    {
        if (index >= tokens.size())
            return Error(ErrorType::InvalidToken, "The command finished but it was expected a " + tokens[index].GetUpperCaseLexeme());

        if (tokens[index].GetToken() != expextedToken)
            return Error(ErrorType::InvalidToken, "It was expected a <HERE> but was found a " + tokens[index].GetUpperCaseLexeme());

        symbolTable->AddNode(tokens[index]);

        index++;

        return std::nullopt;
    }

public:
    SyntaxParser(SyntaxParser &other) = delete;
    SyntaxParser(SyntaxParser &&other) = delete;
    void operator=(const SyntaxParser &) = delete;
};