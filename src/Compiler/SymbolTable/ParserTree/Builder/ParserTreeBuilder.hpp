#pragma once

#include <memory>
#include <stack>
#include <optional>
#include "../ParserTreeNode.hpp"

class ParserTreeBuider
{
private:
    std::shared_ptr<ParserTreeNode> p_root = nullptr;
    std::stack<std::shared_ptr<ParserTreeNode>> m_nonTerminalLevel;
    std::optional<std::shared_ptr<ParserTreeNode>> AddNodeAndGetChildIfExists(ParserToken token);

public:
    ParserTreeBuider();

    void AddNodeInCurrentTier(TokenDefinition token);
    void AddNodeInCurrentTier(NonTerminalToken token);
    void TierUp();
    std::shared_ptr<ParserTreeNode> Build();
};
