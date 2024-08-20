#pragma once

#include "../../Error/error.hpp"
#include <memory>
#include <optional>
#include <variant>
#include <vector>

const int MAX_TREE_CHILDREN = 11;
const int MIN_INNER_NODE_TREE_CHILDREN = MAX_TREE_CHILDREN % 2 == 0 ? MAX_TREE_CHILDREN / 2 : (MAX_TREE_CHILDREN + 1) / 2;

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
    inline bool operator<=(const BTreeKey &key)
    {
        return m_value <= key.m_value;
    }
    inline bool operator>=(const BTreeKey &key)
    {
        return m_value >= key.m_value;
    }
};

class BTree
{
private:
    bool m_isLeaf;
    std::shared_ptr<BTreeInnerNode> p_innerRoot;
    std::shared_ptr<BTreeLeafNode> p_leafRoot;

    std::optional<Error> InsertLeaf(BTreeKey key, int value);

public:
    BTree(); // receive here the factory to know how to generate BTreeLeafNode
    std::optional<Error> Insert(BTreeKey key, int value);
    std::variant<int, Error> FindOne(BTreeKey key);
};

class BTreeInnerNode
{
private:
    bool m_hasLeafChildren;
    std::vector<BTreeKey> p_keys;
    std::vector<std::shared_ptr<BTreeInnerNode>> p_innerChildren;
    std::vector<std::shared_ptr<BTreeLeafNode>> p_leafChildren;

public:
    BTreeInnerNode();
    BTreeInnerNode(BTreeKey key, std::shared_ptr<BTreeLeafNode> leftNode, std::shared_ptr<BTreeLeafNode> rightNode);
    bool GetHasLeafChildren();
    int BinarySearchIndexForNextNode(BTreeKey key);
    std::shared_ptr<BTreeInnerNode> GetInnerNodeByIndex(int index);
    std::shared_ptr<BTreeLeafNode> GetLeafNodeByIndex(int index);
};

class BTreeLeafNode
{
private:
    std::vector<BTreeKey> p_keys;
    // here we could receive the factory to see what it will be saved on leaf page
    std::optional<int> BinarySearchIndexData(BTreeKey key);

public:
    BTreeLeafNode();
    BTreeLeafNode(std::vector<BTreeKey> initKeys);
    int GetKeySize();
    std::optional<int> FindOne(BTreeKey key);
    std::optional<Error> InsertOne(BTreeKey key, int value);
    std::shared_ptr<BTreeInnerNode> Split();
};