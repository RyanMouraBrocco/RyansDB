#pragma once

#include <memory>
#include <optional>
#include "../SymbolTable/SymbolTable.hpp"
#include "./Parsers/SelectParser/SelectParser.hpp"
#include "./Parsers/InsertParser/InsertParser.hpp"
#include "./Parsers/UpdateParser/UpdateParser.hpp"
#include "./Parsers/DeleteParser/DeleteParser.hpp"
#include "./Parsers/AlterTableParser/AlterTableParser.hpp"
#include "./Parsers/CreateDatabaseParser/CreateDatabaseParser.hpp"
#include "./Parsers/DropDatabaseParser/DropDatabaseParser.hpp"
#include "./Parsers/CreateTableParser/CreateTableParser.hpp"
#include "./Parsers/DropTableParser/DropTableParser.hpp"

class SyntaxAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;
    std::shared_ptr<SelectParser> p_selectParser = SelectParser::GetInstance();
    std::shared_ptr<InsertParser> p_insertParser = InsertParser::GetInstance();
    std::shared_ptr<UpdateParser> p_updateParser = UpdateParser::GetInstance();
    std::shared_ptr<DeleteParser> p_deleteParser = DeleteParser::GetInstance();
    std::shared_ptr<AlterTableParser> p_alterTableParser = AlterTableParser::GetInstance();
    std::shared_ptr<CreateDatabaseParser> p_createDatabaseParser = CreateDatabaseParser::GetInstance();
    std::shared_ptr<DropDatabaseParser> p_dropDatabaseParser = DropDatabaseParser::GetInstance();
    std::shared_ptr<CreateTableParser> p_createTableParser = CreateTableParser::GetInstance();
    std::shared_ptr<DropTableParser> p_dropTableParser = DropTableParser::GetInstance();
public:
    SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute();
};