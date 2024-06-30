#pragma once

#include <memory>
#include <mutex>
#include "../Base/CommandParser.hpp"

class DropDatabaseParser : public CommandParser
{
private:
    static std::shared_ptr<DropDatabaseParser> p_singleton;
    static std::mutex m_mutex;

public:
    static std::shared_ptr<DropDatabaseParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};