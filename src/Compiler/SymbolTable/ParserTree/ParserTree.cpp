#include "ParserTree.hpp"

ParserTreeNode::ParserTreeNode()
{
}

ParserTreeNode::ParserTreeNode(const std::shared_ptr<ParserTreeNode> father) : p_father(father)
{
}

void ParserTreeNode::AddChild(const std::shared_ptr<ParserTreeNode> child)
{
    p_children.push_back(child);
}

ParserTree::ParserTree()
{
}

