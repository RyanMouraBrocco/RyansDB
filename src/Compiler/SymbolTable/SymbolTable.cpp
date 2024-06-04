#include "SymbolTable.hpp"

LexemeTokenDefinition::LexemeTokenDefinition(Token token, std::string lowerCaseLexeme, std::string upperCaseLexeme) : m_token(token), m_lowerCaseLexeme(lowerCaseLexeme), m_upperCaseLexeme(upperCaseLexeme)
{
    if (m_lowerCaseLexeme.length() != m_upperCaseLexeme.length())
        throw std::invalid_argument("INVALID LEXEME DEFINITION: lowerCase: '" + m_lowerCaseLexeme + "' and upperCase: '" + m_upperCaseLexeme + "' must have same length");

    m_lexemeLength = m_lowerCaseLexeme.length();
}

Token LexemeTokenDefinition::GetToken() const
{
    return m_token;
}

std::string LexemeTokenDefinition::GetUpperCaseLexeme() const
{
    return m_upperCaseLexeme;
}

int LexemeTokenDefinition::LexemeLength() const
{
    return m_lexemeLength;
}

bool LexemeTokenDefinition::IsValidCharacterInTheIndex(const char &value, const int &index) const
{
    if (index >= m_lowerCaseLexeme.length() || index >= m_upperCaseLexeme.length())
        return false;

    return value == m_lowerCaseLexeme[index] || value == m_upperCaseLexeme[index];
}

SymbolTable::SymbolTable()
{
}

void SymbolTable::AddToken(const std::string value, const Token token)
{
    m_tokens.push_back(std::make_tuple(value, token));
}

bool SymbolTable::IsSpecialCharacterToken(const char &value)
{
    return SymbolTable::m_specialCharacterTokens.find(value) != SymbolTable::m_specialCharacterTokens.end();
}

std::variant<Token, Error> SymbolTable::GetSpecialCharacterToken(const char &value)
{
    if (IsSpecialCharacterToken(value))
        return SymbolTable::m_specialCharacterTokens[value];

    return Error(ErrorType::InvalidCharacter, "Invalid character");
}

std::shared_ptr<std::vector<LexemeTokenDefinition>> SymbolTable::GetReservedStatementsDefinitions()
{
    return SymbolTable::m_reservedStatements;
}

std::shared_ptr<std::vector<LexemeTokenDefinition>> SymbolTable::m_reservedStatements = std::make_shared<std::vector<LexemeTokenDefinition>>(
    std::initializer_list<LexemeTokenDefinition>{
        LexemeTokenDefinition(Token::SELECT, "select", "SELECT"),
        LexemeTokenDefinition(Token::FROM, "from", "FROM"),
        LexemeTokenDefinition(Token::WHERE, "where", "WHERE"),
        LexemeTokenDefinition(Token::NULL_VALUE, "null", "NULL"),
        LexemeTokenDefinition(Token::NOT, "not", "NOT"),
        LexemeTokenDefinition(Token::IS, "is", "IS"),
        LexemeTokenDefinition(Token::INSERT, "insert", "INSERT"),
        LexemeTokenDefinition(Token::UPDATE, "update", "UPDATE"),
        LexemeTokenDefinition(Token::DELETE, "delete", "DELETE"),
        LexemeTokenDefinition(Token::SET, "set", "SET"),
        LexemeTokenDefinition(Token::INTO, "into", "INTO"),
        LexemeTokenDefinition(Token::VALUES, "values", "VALUES"),
        LexemeTokenDefinition(Token::DECLARE, "declare", "DECLARE"),
        LexemeTokenDefinition(Token::INT, "int", "INT"),
        LexemeTokenDefinition(Token::NVARCHAR, "nvarchar", "NVARCHAR"),
        LexemeTokenDefinition(Token::VARCHAR, "varchar", "VARCHAR"),
        LexemeTokenDefinition(Token::BIT, "bit", "BIT"),
        LexemeTokenDefinition(Token::NCHAR, "nchar", "NCHAR"),
        LexemeTokenDefinition(Token::CHAR, "char", "CHAR"),
        LexemeTokenDefinition(Token::PRIMARY, "primary", "PRIMARY"),
        LexemeTokenDefinition(Token::FOREIGN, "foreign", "FOREIGN"),
        LexemeTokenDefinition(Token::KEY, "key", "KEY"),
        LexemeTokenDefinition(Token::CREATE, "create", "CREATE"),
        LexemeTokenDefinition(Token::TABLE, "table", "TABLE"),
        LexemeTokenDefinition(Token::DROP, "drop", "DROP"),
        LexemeTokenDefinition(Token::ALTER, "alter", "ALTER"),
        LexemeTokenDefinition(Token::ADD, "add", "ADD"),
        LexemeTokenDefinition(Token::CONSTRAINT, "constraint", "CONSTRAINT"),
        LexemeTokenDefinition(Token::UNIQUE, "unique", "UNIQUE"),
        LexemeTokenDefinition(Token::INDEX, "index", "INDEX"),
        LexemeTokenDefinition(Token::DATABASE, "database", "DATABASE"),
        LexemeTokenDefinition(Token::AND, "and", "AND"),
        LexemeTokenDefinition(Token::OR, "or", "OR"),
    });

std::map<char, Token> SymbolTable::m_specialCharacterTokens = {
    {'*', Token::ASTERISK},
    {',', Token::COMMA},
    {'.', Token::DOT},
    {';', Token::SEMICOLON},
    {'=', Token::EQUAL},
    {'(', Token::LEFT_PARENTHESIS},
    {')', Token::RIGHT_PARENTHESIS},
};