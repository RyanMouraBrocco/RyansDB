#pragma once

class BTreeKey
{
private:
    int m_value;

public:
    BTreeKey(int value);

    inline bool operator>(const BTreeKey &key)
    {
        return m_value > key.m_value;
    }

    inline bool operator<(const BTreeKey &key)
    {
        return m_value < key.m_value;
    }

    inline bool operator==(const BTreeKey &key)
    {
        return m_value == key.m_value;
    }

    inline bool operator!=(const BTreeKey &key)
    {
        return m_value != key.m_value;
    }

    inline bool operator<=(const BTreeKey &key)
    {
        return m_value <= key.m_value;
    }

    inline bool operator>=(const BTreeKey &key)
    {
        return m_value >= key.m_value;
    }
};