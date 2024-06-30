#pragma once

#include <memory>
#include <mutex>
#include "../Base/SyntaxParser.hpp"

class DropTableParser : public SyntaxParser
{
private:
    static std::shared_ptr<DropTableParser> p_singleton;
    static std::mutex m_mutex;

public:
    static std::shared_ptr<DropTableParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};