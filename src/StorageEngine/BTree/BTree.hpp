#pragma once

#include "../../Error/error.hpp"
#include "Key/BTreeKey.hpp"
#include <memory>
#include <optional>
#include <variant>
#include <vector>

const int MAX_TREE_CHILDREN = 10;
const int MIN_INNER_NODE_TREE_CHILDREN = MAX_TREE_CHILDREN % 2 == 0 ? MAX_TREE_CHILDREN / 2 : (MAX_TREE_CHILDREN + 1) / 2;

class BTreeInnerNode;
class BTreeLeafNode;

class BTree
{
private:
    bool m_isLeaf;
    std::shared_ptr<BTreeInnerNode> p_innerRoot;
    std::shared_ptr<BTreeLeafNode> p_leafRoot;

    std::optional<Error> InsertLeaf(BTreeKey key, int value);
    std::optional<Error> InsertInnerNode(BTreeKey key, int value);

public:
    BTree(); // receive here the factory to know how to generate BTreeLeafNode
    std::optional<Error> Insert(BTreeKey key, int value);
    std::variant<int, Error> FindOne(BTreeKey key);
};

class BTreeInnerNode
{
private:
    bool m_hasLeafChildren;
    std::shared_ptr<BTreeInnerNode> p_father = nullptr;
    std::vector<BTreeKey> p_keys;
    std::vector<std::shared_ptr<BTreeInnerNode>> p_innerChildren;
    std::vector<std::shared_ptr<BTreeLeafNode>> p_leafChildren;

public:
    BTreeInnerNode();
    BTreeInnerNode(std::vector<BTreeKey> keys, std::vector<std::shared_ptr<BTreeInnerNode>> innerNodes, std::vector<std::shared_ptr<BTreeLeafNode>> leafNodes);
    BTreeInnerNode(BTreeKey key, BTreeLeafNode* leftNode, std::shared_ptr<BTreeLeafNode> rightNode);
    BTreeInnerNode(BTreeKey key, BTreeInnerNode *leftNode, std::shared_ptr<BTreeInnerNode> rightNode);
    bool GetHasLeafChildren();
    int GetKeySize();
    int BinarySearchIndexForNextNode(BTreeKey key);
    std::shared_ptr<BTreeInnerNode> GetInnerNodeByIndex(int index);
    std::shared_ptr<BTreeLeafNode> GetLeafNodeByIndex(int index);
    std::shared_ptr<BTreeInnerNode> Split();
};

class BTreeLeafNode
{
private:
    std::shared_ptr<BTreeInnerNode> p_father = nullptr;
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