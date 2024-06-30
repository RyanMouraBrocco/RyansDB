#include "SyntaxAnalyzer.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SyntaxAnalyzer::Execute()
{
    auto tokens = *p_symbolTable->GetSortTokens();
    int index = 0;

    std::optional<Error> errorResult = std::nullopt;
    while (index < tokens.size())
    {
        if (tokens[index].GetToken() == Token::SELECT)
            errorResult = p_selectParser->ParserIntoParserTree(p_symbolTable, tokens, index);
        else if (tokens[index].GetToken() == Token::INSERT)
            errorResult = p_insertParser->ParserIntoParserTree(p_symbolTable, tokens, index);
        else if (tokens[index].GetToken() == Token::UPDATE)
            errorResult = p_updateParser->ParserIntoParserTree(p_symbolTable, tokens, index);
        else if (tokens[index].GetToken() == Token::DELETE)
            errorResult = p_deleteParser->ParserIntoParserTree(p_symbolTable, tokens, index);
        else if (tokens[index].GetToken() == Token::ALTER)
            errorResult = p_alterTableParser->ParserIntoParserTree(p_symbolTable, tokens, index);
        else if (index + 1 < tokens.size())
        {
            if (tokens[index].GetToken() == Token::CREATE && tokens[index + 1].GetToken() == Token::DATABASE)
                errorResult = p_createDatabaseParser->ParserIntoParserTree(p_symbolTable, tokens, index);
            else if (tokens[index].GetToken() == Token::DROP && tokens[index + 1].GetToken() == Token::DATABASE)
                errorResult = p_dropDatabaseParser->ParserIntoParserTree(p_symbolTable, tokens, index);
            else if (tokens[index].GetToken() == Token::CREATE && tokens[index + 1].GetToken() == Token::TABLE)
                errorResult = p_createTableParser->ParserIntoParserTree(p_symbolTable, tokens, index);
            else if (tokens[index].GetToken() == Token::DROP && tokens[index + 1].GetToken() == Token::TABLE)
                errorResult = p_dropTableParser->ParserIntoParserTree(p_symbolTable, tokens, index);
        }

        if (errorResult != std::nullopt)
            return errorResult;
    }

    return std::nullopt;
}