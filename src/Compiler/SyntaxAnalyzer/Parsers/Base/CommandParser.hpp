#pragma once

#include <optional>
#include <memory>
#include "SyntaxParser.hpp"

class CommandParser : public SyntaxParser
{
public:
    virtual std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const = 0;
};