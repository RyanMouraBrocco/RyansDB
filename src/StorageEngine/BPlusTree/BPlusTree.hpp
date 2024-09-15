#pragma once

#include "../../Error/error.hpp"
#include "InnerNode/BPlusTreeInnerNode.hpp"
#include "LeafNode/BPlusTreeLeafNode.hpp"
#include "Key/BPlusTreeKey.hpp"
#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include <stack>

const int MAX_TREE_CHILDREN = 7; // HERE MUST BE AN ODD NUMBER
const int MIN_TREE_CHILDREN = MAX_TREE_CHILDREN / 2;

class BPlusTree
{
private:
    bool m_isLeaf;
    BPlusTreeInnerNode *p_innerRoot;
    BPlusTreeLeafNode *p_leafRoot;

    std::optional<Error> InsertLeaf(BPlusTreeKey key, int value);
    std::optional<Error> InsertInnerNode(BPlusTreeKey key, int value);
    std::optional<Error> DeleteLeaf(BPlusTreeKey key);
    std::optional<Error> DeleteInnerNode(BPlusTreeKey key);

public:
    BPlusTree(); // receive here the factory to know how to generate BPlusTreeLeafNode
    std::optional<Error> Insert(BPlusTreeKey key, int value);
    std::optional<Error> Delete(BPlusTreeKey key);
    std::variant<int, Error> FindOne(BPlusTreeKey key);
    std::vector<BPlusTreeKey> ListAll();
};
