#include "SymbolTable.hpp"

SymbolTable::SymbolTable()
{
}

void SymbolTable::AddToken(const std::string value, const Token token)
{
    m_tokens.push_back(std::make_tuple(value, token));
}