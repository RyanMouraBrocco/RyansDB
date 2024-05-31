#pragma onde;

#include <memory>
#include "../SymbolTable/SymbolTable.hpp"
#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <map>

class LexycalAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    bool IsEndTokenChar(const char &value) const;
    bool IsReservedStatement(std::string &query, int &index, int &queryLength);
    bool IsStatement(const std::string &&expectedStatement, const std::string &query, const int &index) const;
    bool SaveStringStatement(const std::string &query, int &index);
    bool SaveVariableStatement(const std::string &query, int &index);
    bool SaveIdentifierStatement(const std::string &query, int &index);
    bool IsValidSpecialCharacter(const std::string &query, int &index);
    bool SaveNumberStatement(const std::string &query, int &index);

public:
    LexycalAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    bool Execute(std::string query);
};