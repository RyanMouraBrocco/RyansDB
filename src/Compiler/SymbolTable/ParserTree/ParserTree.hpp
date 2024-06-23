#pragma once

#include <vector>
#include <memory>
#include <stack>
#include <optional>
#include "../TokenDefinition/TokenDefinition.hpp"

enum class ParserType
{
    Non_Terminal,
    Terminal,
};

class ParserToken
{
private:
    ParserType m_type;
    std::optional<TokenDefinition> m_tokenDefintion = std::nullopt;
    std::optional<NonTerminalToken> m_nonTerminalToken = std::nullopt;

public:
    ParserToken(TokenDefinition tokenDefinition);
    ParserToken(NonTerminalToken nonTerminalToken);
};

class ParserTreeNode
{
private:
    std::shared_ptr<ParserTreeNode> p_father = nullptr;
    std::vector<std::shared_ptr<ParserTreeNode>> p_children;
    ParserToken m_token;

public:
    ParserTreeNode(std::shared_ptr<ParserTreeNode> father, ParserToken token);
    ParserTreeNode(ParserToken token);

    void AddChild(const std::shared_ptr<ParserTreeNode> child);
};

class ParserTree
{
private:
    std::shared_ptr<ParserTreeNode> p_root = nullptr;
    std::stack<std::shared_ptr<ParserTreeNode>> m_nonTerminalLevel;

public:
    ParserTree();

    void AddNodeInCurrentTier(TokenDefinition token);
    void AddNodeInCurrentTier(NonTerminalToken token);
    void TierUp();
};
