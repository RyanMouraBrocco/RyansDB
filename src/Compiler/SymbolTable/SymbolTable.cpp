#include "SymbolTable.hpp"

TokenDefinition::TokenDefinition(Token token, std::string lexeme)
{
    std::transform(lexeme.begin(), lexeme.end(), m_lowerCaseLexeme.begin(), ::tolower);
    std::transform(lexeme.begin(), lexeme.end(), m_upperCaseLexeme.begin(), ::toupper);
    if (m_lowerCaseLexeme.length() != m_upperCaseLexeme.length())
        throw std::invalid_argument("INVALID LEXEME DEFINITION: lowerCase: '" + m_lowerCaseLexeme + "' and upperCase: '" + m_upperCaseLexeme + "' must have same length");

    m_lexemeLength = m_lowerCaseLexeme.length();
}

TokenDefinition::TokenDefinition(Token token, std::string lowerCaseLexeme, std::string upperCaseLexeme) : m_token(token), m_lowerCaseLexeme(lowerCaseLexeme), m_upperCaseLexeme(upperCaseLexeme)
{
    if (m_lowerCaseLexeme.length() != m_upperCaseLexeme.length())
        throw std::invalid_argument("INVALID LEXEME DEFINITION: lowerCase: '" + m_lowerCaseLexeme + "' and upperCase: '" + m_upperCaseLexeme + "' must have same length");

    m_lexemeLength = m_lowerCaseLexeme.length();
}

Token TokenDefinition::GetToken() const
{
    return m_token;
}

std::string TokenDefinition::GetUpperCaseLexeme() const
{
    return m_upperCaseLexeme;
}

int TokenDefinition::LexemeLength() const
{
    return m_lexemeLength;
}

bool TokenDefinition::IsValidCharacterInTheIndex(const char &value, const int &index) const
{
    if (index >= m_lowerCaseLexeme.length() || index >= m_upperCaseLexeme.length())
        return false;

    return value == m_lowerCaseLexeme[index] || value == m_upperCaseLexeme[index];
}

SymbolTable::SymbolTable()
{
    p_tokens = std::make_shared<std::vector<TokenDefinition>>();
}

void SymbolTable::AddToken(const std::string value, const Token token)
{
    p_tokens->push_back(TokenDefinition(token, value));
}

std::shared_ptr<std::vector<TokenDefinition>> SymbolTable::GetSortTokens() const
{
    return p_tokens;
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

std::shared_ptr<std::vector<TokenDefinition>> SymbolTable::GetReservedStatementsDefinitions()
{
    return SymbolTable::m_reservedStatements;
}

std::shared_ptr<std::vector<TokenDefinition>> SymbolTable::m_reservedStatements = std::make_shared<std::vector<TokenDefinition>>(
    std::initializer_list<TokenDefinition>{
        TokenDefinition(Token::SELECT, "select", "SELECT"),
        TokenDefinition(Token::FROM, "from", "FROM"),
        TokenDefinition(Token::WHERE, "where", "WHERE"),
        TokenDefinition(Token::NULL_VALUE, "null", "NULL"),
        TokenDefinition(Token::NOT, "not", "NOT"),
        TokenDefinition(Token::IS, "is", "IS"),
        TokenDefinition(Token::INSERT, "insert", "INSERT"),
        TokenDefinition(Token::UPDATE, "update", "UPDATE"),
        TokenDefinition(Token::DELETE, "delete", "DELETE"),
        TokenDefinition(Token::SET, "set", "SET"),
        TokenDefinition(Token::INTO, "into", "INTO"),
        TokenDefinition(Token::VALUES, "values", "VALUES"),
        TokenDefinition(Token::DECLARE, "declare", "DECLARE"),
        TokenDefinition(Token::INT, "int", "INT"),
        TokenDefinition(Token::NVARCHAR, "nvarchar", "NVARCHAR"),
        TokenDefinition(Token::VARCHAR, "varchar", "VARCHAR"),
        TokenDefinition(Token::BIT, "bit", "BIT"),
        TokenDefinition(Token::NCHAR, "nchar", "NCHAR"),
        TokenDefinition(Token::CHAR, "char", "CHAR"),
        TokenDefinition(Token::PRIMARY, "primary", "PRIMARY"),
        TokenDefinition(Token::FOREIGN, "foreign", "FOREIGN"),
        TokenDefinition(Token::KEY, "key", "KEY"),
        TokenDefinition(Token::CREATE, "create", "CREATE"),
        TokenDefinition(Token::TABLE, "table", "TABLE"),
        TokenDefinition(Token::DROP, "drop", "DROP"),
        TokenDefinition(Token::ALTER, "alter", "ALTER"),
        TokenDefinition(Token::ADD, "add", "ADD"),
        TokenDefinition(Token::CONSTRAINT, "constraint", "CONSTRAINT"),
        TokenDefinition(Token::UNIQUE, "unique", "UNIQUE"),
        TokenDefinition(Token::INDEX, "index", "INDEX"),
        TokenDefinition(Token::DATABASE, "database", "DATABASE"),
        TokenDefinition(Token::AND, "and", "AND"),
        TokenDefinition(Token::OR, "or", "OR"),
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