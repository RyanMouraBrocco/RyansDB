#include "BTree.hpp"

BTree::BTree()
{
    m_isLeaf = true;
}

std::optional<Error> BTree::Insert(BTreeKey key, int value)
{
}

std::variant<int, Error> BTree::FindOne(BTreeKey key)
{
    if (m_isLeaf)
    {
        auto leafValue = p_leafRoot->FindOne(key);
        if (leafValue.has_value())
            return leafValue.value();
    }
    else
    {
        auto next = p_innerRoot;
        while (next != nullptr)
        {
            int keyIndex = next->BinarySearchIndexForNextNode(key);
            if (next->GetHasLeafChildren())
            {
                auto leafNode = next->GetLeafNodeByIndex(keyIndex);
                auto leafValue = leafNode->FindOne(key);
                if (leafValue.has_value())
                    return leafValue.value();
            }
            else
                next = next->GetInnerNodeByIndex(keyIndex);
        }
    }

    return Error(ErrorType::Unexpected, "Unexpect behavior when get in btree");
}

int BTreeInnerNode::BinarySearchIndexForNextNode(BTreeKey key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BTreeKey middleKey = *p_keys[middle];
        if (middleKey > key)
        {
            if (middle < lastItem && *p_keys[middle + 1] <= key)
                return middle;

            min = middle + 1;
        }
        else if (middleKey < key)
        {
            if (middle > 0 && *p_keys[middle - 1] >= key)
                return middle;

            max = middle - 1;
        }
        else
            return middle;
    }

    return lastItem + 1;
}

std::optional<int> BTreeLeafNode::FindOne(BTreeKey key)
{
    auto indexOfItem = BinarySearchIndexData(key);
    if (!indexOfItem.has_value())
        return indexOfItem;

    // use here to access database and fetch data
    // for now, its just return the index
    return indexOfItem.value();
}

std::optional<int> BTreeLeafNode::BinarySearchIndexData(BTreeKey key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BTreeKey middleKey = *p_keys[middle];
        if (middleKey > key)
            min = middle + 1;
        else if (middleKey < key)
            max = middle - 1;
        else
            return middle;
    }

    return std::nullopt;
}