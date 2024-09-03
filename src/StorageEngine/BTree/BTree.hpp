#pragma once

#include "../../Error/error.hpp"
#include "Key/BTreeKey.hpp"
#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include <stack>

const int MAX_TREE_CHILDREN = 3;

class BTreeInnerNode;
class BTreeLeafNode;

class BTree
{
private:
    bool m_isLeaf;
    BTreeInnerNode *p_innerRoot;
    BTreeLeafNode *p_leafRoot;

    std::optional<Error> InsertLeaf(BTreeKey key, int value);
    std::optional<Error> InsertInnerNode(BTreeKey key, int value);
    std::optional<Error> DeleteLeaf(BTreeKey key);
    std::optional<Error> DeleteInnerNode(BTreeKey key);

public:
    BTree(); // receive here the factory to know how to generate BTreeLeafNode
    std::optional<Error> Insert(BTreeKey key, int value);
    std::optional<Error> Delete(BTreeKey key);
    std::variant<int, Error> FindOne(BTreeKey key);
    std::vector<BTreeKey> ListAll();
};

class BTreeInnerNode
{
private:
    bool m_hasLeafChildren;
    BTreeInnerNode *p_father = nullptr;
    std::vector<BTreeKey> p_keys;
    std::vector<BTreeInnerNode *> p_innerChildren;
    std::vector<BTreeLeafNode *> p_leafChildren;

public:
    BTreeInnerNode();
    BTreeInnerNode(std::vector<BTreeKey> keys, std::vector<BTreeInnerNode *> innerNodes, std::vector<BTreeLeafNode *> leafNodes);
    BTreeInnerNode(BTreeKey key, BTreeLeafNode *leftNode, BTreeLeafNode *rightNode);
    BTreeInnerNode(BTreeKey key, BTreeInnerNode *leftNode, BTreeInnerNode *rightNode);
    bool GetHasLeafChildren();
    int GetKeySize();
    int BinarySearchIndexForNextNode(BTreeKey key);
    BTreeInnerNode *GetInnerNodeByIndex(int index);
    BTreeLeafNode *GetLeafNodeByIndex(int index);
    BTreeInnerNode *Split();
    void InsertOne(BTreeKey key, BTreeLeafNode *rightNode);
    void InsertOne(BTreeKey key, BTreeInnerNode *rightNode);
    BTreeKey GetKey(int index);
    void UpdateKey(int index, BTreeKey key);
};

class BTreeLeafNode
{
private:
    BTreeInnerNode *p_father = nullptr;
    BTreeLeafNode *p_nextPage = nullptr;
    BTreeLeafNode *p_previousPage = nullptr;
    std::vector<BTreeKey> p_keys;
    // here we could receive the factory to see what it will be saved on leaf page
    std::optional<int> BinarySearchIndexData(BTreeKey key);

public:
    BTreeLeafNode();
    BTreeLeafNode(std::vector<BTreeKey> initKeys);
    int GetKeySize();
    std::optional<int> FindOne(BTreeKey key);
    std::optional<Error> InsertOne(BTreeKey key, int value);
    std::optional<Error> DeleteOne(BTreeKey key);
    BTreeInnerNode *Split();
    void SetFather(BTreeInnerNode *father);
    BTreeKey GetKey(int index);
    BTreeLeafNode *GetNextPage();
};