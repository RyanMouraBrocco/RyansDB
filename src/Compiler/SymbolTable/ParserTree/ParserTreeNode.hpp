#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <variant>
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

    std::variant<TokenDefinition, NonTerminalToken> GetValue() const;
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
    std::vector<std::shared_ptr<ParserTreeNode>> GetChildren() const;
    ParserToken GetToken() const;
};