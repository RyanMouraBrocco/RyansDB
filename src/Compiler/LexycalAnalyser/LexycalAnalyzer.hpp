#pragma onde;

#include <memory>
#include "../SymbolTable/SymbolTable.hpp"
#include <sstream>
#include <ctype.h>

class LexycalAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    bool IsEndTokenChar(const char &value) const;
    bool IsReservedStatement(std::string &query, int &index, int &queryLength);
    bool IsStatement(const std::string &&expectedStatement, const std::string &query, const int &index) const;

public:
    LexycalAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    bool Execute(std::string query);
};