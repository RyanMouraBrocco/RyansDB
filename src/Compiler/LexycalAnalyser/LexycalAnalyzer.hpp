#pragma once

#include <memory>
#include "../SymbolTable/SymbolTable.hpp"
#include "../../Error/error.hpp"
#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <map>
#include <optional>
#include <tuple>

class LexycalAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    bool IsSkippableChar(const char &value) const;
    bool IsEndTokenChar(const char &value) const;
    bool IsStatement(const std::string &expectedStatement, const std::string &query, const int &index) const;

    std::optional<Error> SaveAlphaStatement(const std::string &query, int &index);
    std::optional<Error> SaveReservedStatement(const std::string &query, int &index);
    std::optional<Error> SaveIdentifierStatement(const std::string &query, int &index);
    std::optional<Error> SaveNumberStatement(const std::string &query, int &index);
    std::optional<Error> SaveVariableStatement(const std::string &query, int &index);
    std::optional<Error> SaveStringStatement(const std::string &query, int &index);
    std::optional<Error> SaveSpecialCharacter(const std::string &query, int &index);

public:
    LexycalAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute(std::string query);
};