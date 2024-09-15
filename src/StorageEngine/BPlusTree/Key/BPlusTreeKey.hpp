#pragma once

class BPlusTreeKey
{
private:
    int m_value;

public:
    BPlusTreeKey(int value);

    inline bool operator>(const BPlusTreeKey &key)
    {
        return m_value > key.m_value;
    }

    inline bool operator<(const BPlusTreeKey &key)
    {
        return m_value < key.m_value;
    }

    inline bool operator==(const BPlusTreeKey &key)
    {
        return m_value == key.m_value;
    }

    inline bool operator!=(const BPlusTreeKey &key)
    {
        return m_value != key.m_value;
    }

    inline bool operator<=(const BPlusTreeKey &key)
    {
        return m_value <= key.m_value;
    }

    inline bool operator>=(const BPlusTreeKey &key)
    {
        return m_value >= key.m_value;
    }
};