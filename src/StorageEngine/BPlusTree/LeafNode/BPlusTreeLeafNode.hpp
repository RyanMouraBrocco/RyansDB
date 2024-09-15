#pragma once

#include <vector>
#include <optional>
#include <stack>
#include "../Key/BPlusTreeKey.hpp"
#include "../../../Error/error.hpp"
#include "../InnerNode/BPlusTreeInnerNode.hpp"

class BPlusTreeInnerNode;

class BPlusTreeLeafNode
{
private:
    BPlusTreeInnerNode *p_father = nullptr;
    BPlusTreeLeafNode *p_nextPage = nullptr;
    BPlusTreeLeafNode *p_previousPage = nullptr;
    std::vector<BPlusTreeKey> p_keys;
    // here we could receive the factory to see what it will be saved on leaf page

    BPlusTreeLeafNode(std::vector<BPlusTreeKey> initKeys);
    std::optional<int> BinarySearchIndexData(BPlusTreeKey key);

public:
    BPlusTreeLeafNode();

    // GETTERS AND SETTERS
    int GetKeySize();
    BPlusTreeKey GetKey(int index);
    BPlusTreeInnerNode *GetFather();
    void SetFather(BPlusTreeInnerNode *father);
    BPlusTreeLeafNode *GetNextPage();
    BPlusTreeLeafNode *GetPreviousPage();
    // GETTERS AND SETTERS

    std::optional<int> FindOne(BPlusTreeKey key);
    std::optional<Error> InsertOne(BPlusTreeKey key, int value);
    std::optional<Error> DeleteOne(BPlusTreeKey key);
    BPlusTreeInnerNode *Split();
    std::optional<Error> BorrowFromNextPage();
    std::optional<Error> BorrowFromPreviousPage();
    std::optional<Error> MergeWithNextPage();
};