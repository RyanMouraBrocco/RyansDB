#pragma once

#include <vector>
#include <memory>

class ParserTreeNode
{
private:
    std::shared_ptr<ParserTreeNode> p_father = nullptr;
    std::vector<std::shared_ptr<ParserTreeNode>> p_children;

public:
    ParserTreeNode(const std::shared_ptr<ParserTreeNode> father);
    ParserTreeNode();

    void AddChild(const std::shared_ptr<ParserTreeNode> child);
};

class ParserTree
{
private:
    std::shared_ptr<ParserTreeNode> p_root;

public:
    ParserTree();
};
