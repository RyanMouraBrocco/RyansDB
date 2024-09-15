#pragma once

#include <vector>
#include <variant>
#include "../LeafNode/BPlusTreeLeafNode.hpp"
#include "../Key/BPlusTreeKey.hpp"
#include "../../../Error/error.hpp"

class BPlusTreeLeafNode;

class BPlusTreeInnerNode
{
private:
    bool m_hasLeafChildren;
    BPlusTreeInnerNode *p_father = nullptr;
    std::vector<BPlusTreeKey> p_keys;
    std::vector<BPlusTreeInnerNode *> p_innerChildren;
    std::vector<BPlusTreeLeafNode *> p_leafChildren;

    BPlusTreeInnerNode(std::vector<BPlusTreeKey> keys, std::vector<BPlusTreeInnerNode *> innerNodes, std::vector<BPlusTreeLeafNode *> leafNodes);
    BPlusTreeInnerNode(BPlusTreeKey key, BPlusTreeInnerNode *leftNode, BPlusTreeInnerNode *rightNode);

    BPlusTreeKey GetKey(int index);
    int GetLeafNodeSize();
    void DeleteInnerChildrenByIndex(int index);

    void InsertOne(BPlusTreeKey key, BPlusTreeInnerNode *rightNode);

public:
    BPlusTreeInnerNode(BPlusTreeKey key, BPlusTreeLeafNode *leftNode, BPlusTreeLeafNode *rightNode);

    // GETTERS AND SETTERS
    bool GetHasLeafChildren();
    int GetKeySize();
    void DeleteKeyByIndex(int index);
    void UpdateKey(int index, BPlusTreeKey key);
    std::variant<int, Error> GetKeyIndex(BPlusTreeKey key);
    BPlusTreeInnerNode *GetFather();
    void SetFather(BPlusTreeInnerNode *father);
    int GetInnerNodeSize();
    std::variant<int, Error> GetInnerNodeIndex(BPlusTreeInnerNode *innerNode);
    BPlusTreeInnerNode *GetInnerNodeByIndex(int index);
    std::variant<int, Error> GetLeafNodeIndex(BPlusTreeLeafNode *leafNode);
    BPlusTreeLeafNode *GetLeafNodeByIndex(int index);
    void DeleteLeafChildrenByIndex(int index);
    // GETTERS AND SETTERS

    int BinarySearchIndexForNextNode(BPlusTreeKey key);
    BPlusTreeInnerNode *Split();
    void InsertOne(BPlusTreeKey key, BPlusTreeLeafNode *rightNode);
    void BorrowFromRightNode(int currentFatherPosition, BPlusTreeInnerNode *rightNode);
    void BorrowFromLeftNode(int currentFatherPosition, BPlusTreeInnerNode *leftNode);
    void MergeWithRightNode(int currentFatherPosition, BPlusTreeInnerNode *rightNode);
};