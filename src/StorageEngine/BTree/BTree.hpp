#pragma once

#include "../../Error/error.hpp"
#include <memory>
#include <optional>

const int MAX_TREE_CHILDREN = 10;
const int MIN_INNER_NODE_TREE_CHILDREN = MAX_TREE_CHILDREN / 2;

class IBTreeNode
{
};

class IBTreeKey
{
};

class BTree
{
private:
    std::shared_ptr<IBTreeNode> p_root;

public:
    BTree(); // receive here the factory to know how to generate BTreeLeafNode
    std::optional<Error> Insert(IBTreeKey key, int value);
};

class BTreeInnerNode : public IBTreeNode
{
private:
    std::shared_ptr<IBTreeKey> p_keys;
    std::shared_ptr<IBTreeNode> p_children;
};

class BTreeLeafNode : public IBTreeNode
{
private:
    // here we could receive the factory to see what it will be saved on leaf page
public:
};