#include "InsertCommand.hpp"

InsertData::InsertData(std::string stringData) : m_stringData(stringData)
{
}

InsertData::InsertData(int integerData) : m_integerData(integerData)
{
}

InsertData::InsertData(double decimalData) : m_decimalData(decimalData)
{
}

InsertData::InsertData(bool booleanData) : m_booleanData(booleanData)
{
}

InsertCommand::InsertCommand(int tableId, std::vector<InsertData> datas) : m_tableId(tableId), m_datas(datas)
{
}