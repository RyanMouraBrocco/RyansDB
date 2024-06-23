#include "TokenDefinition.hpp"

TokenDefinition::TokenDefinition(Token token, std::string lexeme) : m_token(token)
{
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
    m_lowerCaseLexeme = lexeme;
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::toupper);
    m_upperCaseLexeme = lexeme;
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