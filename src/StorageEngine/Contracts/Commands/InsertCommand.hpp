#pragma once

#include "string"
#include "vector"

class InsertData
{
private:
    std::string m_stringData;
    int m_integerData;
    double m_decimalData;
    bool m_booleanData;

    int m_typeLength;

public:
    InsertData(std::string stringData);
    InsertData(int integerData);
    InsertData(double decimalData);
    InsertData(bool booleanData);
};

class InsertCommand
{
private:
    int m_tableId;
    std::vector<InsertData> m_datas;

public:
    InsertCommand(int tableId, std::vector<InsertData> datas);

    int GetAllDataLength();
};