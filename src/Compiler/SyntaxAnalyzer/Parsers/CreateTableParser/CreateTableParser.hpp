#pragma once

#include <memory>
#include <mutex>
#include "../Base/CommandParser.hpp"
#include "../Utils/UtilsParser.hpp"

class CreateTableParser : public CommandParser
{
private:
    static std::shared_ptr<CreateTableParser> p_singleton;
    static std::mutex m_mutex;

    std::shared_ptr<UtilsParser> p_utilsParser = UtilsParser::GetInstance();

    std::optional<Error> CheckCreateTableStatement(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnsDefinitions(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<CreateTableParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};