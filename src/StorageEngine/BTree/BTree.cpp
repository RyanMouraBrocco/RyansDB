#include "BTree.hpp"

BTree::BTree()
{
    m_isLeaf = true;
    p_innerRoot = nullptr;
    p_leafRoot = new BTreeLeafNode();
}

std::optional<Error> BTree::Insert(BTreeKey key, int value)
{
    if (m_isLeaf)
        return InsertLeaf(key, value);

    return InsertInnerNode(key, value);
}

std::optional<Error> BTree::InsertLeaf(BTreeKey key, int value)
{
    auto insertResult = p_leafRoot->InsertOne(key, value);
    if (insertResult.has_value())
        return insertResult;

    if (p_leafRoot->GetKeySize() == MAX_TREE_CHILDREN)
    {
        BTreeInnerNode *innerNode = p_leafRoot->Split();
        p_leafRoot = nullptr;
        p_innerRoot = innerNode;
        m_isLeaf = false;
    }

    return std::nullopt;
}

std::optional<Error> BTree::InsertInnerNode(BTreeKey key, int value)
{
    auto next = p_innerRoot;
    while (next != nullptr)
    {
        int keyIndex = next->BinarySearchIndexForNextNode(key);
        if (next->GetHasLeafChildren())
        {
            auto leafNode = next->GetLeafNodeByIndex(keyIndex);
            auto insertResult = leafNode->InsertOne(key, value);
            if (insertResult.has_value())
                return insertResult;

            if (leafNode->GetKeySize() == MAX_TREE_CHILDREN)
                leafNode->Split();

            auto previous = next;
            while (previous->GetKeySize() == MAX_TREE_CHILDREN)
            {
                bool isRoot = previous == p_innerRoot;
                previous = previous->Split();
                if (isRoot)
                    p_innerRoot = previous;
            }

            next = nullptr;
        }
        else
            next = next->GetInnerNodeByIndex(keyIndex);
    }

    return std::nullopt;
}

std::variant<int, Error> BTree::FindOne(BTreeKey key)
{
    if (m_isLeaf)
    {
        auto leafValue = p_leafRoot->FindOne(key);
        if (leafValue.has_value())
            return leafValue.value();
    }
    else
    {
        auto next = p_innerRoot;
        while (next != nullptr)
        {
            int keyIndex = next->BinarySearchIndexForNextNode(key);
            if (next->GetHasLeafChildren())
            {
                auto leafNode = next->GetLeafNodeByIndex(keyIndex);
                auto leafValue = leafNode->FindOne(key);
                if (leafValue.has_value())
                    return leafValue.value();
            }
            else
                next = next->GetInnerNodeByIndex(keyIndex);
        }
    }

    return Error(ErrorType::Unexpected, "Unexpect behavior when get in btree");
}

BTreeInnerNode::BTreeInnerNode()
{
}

BTreeInnerNode::BTreeInnerNode(std::vector<BTreeKey> keys, std::vector<BTreeInnerNode *> innerNodes, std::vector<BTreeLeafNode *> leafNodes) : p_keys(keys), p_innerChildren(innerNodes), p_leafChildren(leafNodes)
{
    m_hasLeafChildren = leafNodes.size() > 0;
}

BTreeInnerNode::BTreeInnerNode(BTreeKey key, BTreeLeafNode *leftNode, BTreeLeafNode *rightNode)
{
    p_keys.push_back(key);
    p_leafChildren.push_back(leftNode);
    p_leafChildren.push_back(rightNode);
    m_hasLeafChildren = true;
}

BTreeInnerNode::BTreeInnerNode(BTreeKey key, BTreeInnerNode *leftNode, BTreeInnerNode *rightNode)
{
    p_keys.push_back(key);
    p_innerChildren.push_back(leftNode);
    p_innerChildren.push_back(rightNode);
    m_hasLeafChildren = false;
}

BTreeInnerNode *BTreeInnerNode::Split()
{
    int middleNumber = p_keys.size() / 2;

    std::stack<BTreeKey> stackKeys;
    std::stack<BTreeLeafNode *> stackLeafNodes;
    std::stack<BTreeInnerNode *> stackInnerNodes;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        stackKeys.push(p_keys[i]);
        p_keys.pop_back();
    }

    if (m_hasLeafChildren)
    {
        for (int i = p_leafChildren.size() - 1; i >= middleNumber; i--)
        {
            stackLeafNodes.push(p_leafChildren[i]);
            p_leafChildren.pop_back();
        }
    }
    else
    {
        for (int i = p_innerChildren.size() - 1; i >= middleNumber; i--)
        {
            stackInnerNodes.push(p_innerChildren[i]);
            p_innerChildren.pop_back();
        }
    }

    std::vector<BTreeKey> rightArray;
    std::vector<BTreeLeafNode *> rightLeafNodes;
    std::vector<BTreeInnerNode *> rightInnerNodes;
    while (stackKeys.size() > 0)
    {
        rightArray.push_back(stackKeys.top());
        stackKeys.pop();
    }

    while (stackLeafNodes.size() > 0)
    {
        rightLeafNodes.push_back(stackLeafNodes.top());
        stackLeafNodes.pop();
    }

    while (stackInnerNodes.size() > 0)
    {
        rightInnerNodes.push_back(stackInnerNodes.top());
        stackInnerNodes.pop();
    }

    BTreeInnerNode *rightNode = new BTreeInnerNode(rightArray, rightInnerNodes, rightLeafNodes);
    if (p_father == nullptr)
        p_father = new BTreeInnerNode(rightArray[0], this, rightNode);
    else
        p_father->InsertOne(rightArray[0], rightNode);

    rightNode->p_father = p_father;

    return p_father;
}

int BTreeInnerNode::BinarySearchIndexForNextNode(BTreeKey key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BTreeKey middleKey = p_keys[middle];
        if (middleKey >= key)
        {
            if (middle < lastItem && p_keys[middle + 1] < key)
                return middle + 1;

            min = middle + 1;
        }
        else
        {
            if (middle > 0 && p_keys[middle - 1] >= key)
                return middle - 1;

            max = middle - 1;
        }
    }

    return lastItem + 1;
}

bool BTreeInnerNode::GetHasLeafChildren()
{
    return m_hasLeafChildren;
}

int BTreeInnerNode::GetKeySize()
{
    return p_keys.size();
}

BTreeInnerNode *BTreeInnerNode::GetInnerNodeByIndex(int index)
{
    return p_innerChildren[index];
}

BTreeLeafNode *BTreeInnerNode::GetLeafNodeByIndex(int index)
{
    return p_leafChildren[index];
}

void BTreeInnerNode::InsertOne(BTreeKey key, BTreeLeafNode *rightNode)
{
    p_keys.push_back(key);
    p_leafChildren.push_back(rightNode);

    int rightPosition = 1, keyPosition = 0;
    for (int i = p_keys.size() - 2; i >= 0; i--)
    {
        if (p_keys[i] > key)
            p_keys[i + 1] = p_keys[i];
        else
        {
            keyPosition = i + 1;
            rightPosition = i + 2;
            break;
        }
    }

    p_keys[keyPosition] = key;

    for (int i = p_leafChildren.size() - 2; i >= rightPosition; i--)
    {
        p_leafChildren[i + 1] = p_leafChildren[i];
    }

    p_leafChildren[rightPosition] = rightNode;
}

void BTreeInnerNode::InsertOne(BTreeKey key, BTreeInnerNode *rightNode)
{
    p_keys.push_back(key);
    p_innerChildren.push_back(rightNode);

    int rightPosition = 1, keyPosition = 0;
    for (int i = p_keys.size() - 2; i >= 0; i--)
    {
        if (p_keys[i] > key)
            p_keys[i + 1] = p_keys[i];
        else
        {
            keyPosition = i + 1;
            rightPosition = i + 2;
            break;
        }
    }

    p_keys[keyPosition] = key;

    for (int i = p_innerChildren.size() - 2; i >= rightPosition; i--)
    {
        p_innerChildren[i + 1] = p_innerChildren[i];
    }

    p_innerChildren[rightPosition] = rightNode;
}

BTreeLeafNode::BTreeLeafNode()
{
}

BTreeLeafNode::BTreeLeafNode(std::vector<BTreeKey> keys) : p_keys(keys)
{
}

std::optional<int> BTreeLeafNode::BinarySearchIndexData(BTreeKey key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BTreeKey middleKey = p_keys[middle];
        if (middleKey > key)
            min = middle + 1;
        else if (middleKey < key)
            max = middle - 1;
        else
            return middle;
    }

    return std::nullopt;
}

int BTreeLeafNode::GetKeySize()
{
    return p_keys.size();
}

std::optional<int> BTreeLeafNode::FindOne(BTreeKey key)
{
    auto indexOfItem = BinarySearchIndexData(key);
    if (!indexOfItem.has_value())
        return indexOfItem;

    // use here to access database and fetch data
    // for now, its just return the index
    return indexOfItem.value();
}

std::optional<Error> BTreeLeafNode::InsertOne(BTreeKey key, int value)
{
    p_keys.push_back(key);

    int nextPosition = 0;
    for (int i = p_keys.size() - 2; i >= 0; i--)
    {
        if (p_keys[i] > key)
            p_keys[i + 1] = p_keys[i];
        else
        {
            nextPosition = i + 1;
            break;
        }
    }

    p_keys[nextPosition] = key;

    // insert value here;

    return std::nullopt;
}

BTreeInnerNode *BTreeLeafNode::Split()
{
    int middleNumber = p_keys.size() / 2;
    std::stack<BTreeKey> stackKeys;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        stackKeys.push(p_keys[i]);
        p_keys.pop_back();
    }

    std::vector<BTreeKey> rightArray;
    while (stackKeys.size() > 0)
    {
        rightArray.push_back(stackKeys.top());
        stackKeys.pop();
    }

    BTreeLeafNode *rightNode = new BTreeLeafNode(rightArray);
    if (p_father == nullptr)
        p_father = new BTreeInnerNode(rightArray[0], this, rightNode);
    else
        p_father->InsertOne(rightArray[0], rightNode);

    rightNode->p_father = p_father;

    return p_father;
}
