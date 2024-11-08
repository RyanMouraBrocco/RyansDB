#pragma once

template <typename T>
class BPlusTreeKey
{
private:
    T m_value;

public:
    BPlusTreeKey(T value);

    inline bool operator>(const BPlusTreeKey<T> &key)
    {
        return m_value > key.m_value;
    }

    inline bool operator<(const BPlusTreeKey<T> &key)
    {
        return m_value < key.m_value;
    }

    inline bool operator==(const BPlusTreeKey<T> &key)
    {
        return m_value == key.m_value;
    }

    inline bool operator!=(const BPlusTreeKey<T> &key)
    {
        return m_value != key.m_value;
    }

    inline bool operator<=(const BPlusTreeKey<T> &key)
    {
        return m_value <= key.m_value;
    }

    inline bool operator>=(const BPlusTreeKey<T> &key)
    {
        return m_value >= key.m_value;
    }
};

template <typename T>
BPlusTreeKey<T>::BPlusTreeKey(T value) : m_value(value)
{
}