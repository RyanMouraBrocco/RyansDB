#include "BTree.hpp"

BTree::BTree()
{
    m_isLeaf = true;
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
        std::shared_ptr<BTreeInnerNode> innerNode = p_leafRoot->Split();
        p_leafRoot = nullptr;
        p_innerRoot = innerNode;
        m_isLeaf = false;
    }
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
                p_leafRoot->Split();

            auto previous = next;
            while (previous->GetKeySize() == MAX_TREE_CHILDREN)
            {
                previous = previous->Split();
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

BTreeInnerNode::BTreeInnerNode(BTreeKey key, std::shared_ptr<BTreeLeafNode> leftNode, std::shared_ptr<BTreeLeafNode> rightNode)
{
    p_keys.push_back(key);
    p_leafChildren.push_back(leftNode);
    p_leafChildren.push_back(rightNode);
    m_hasLeafChildren = true;
}

std::shared_ptr<BTreeInnerNode> BTreeInnerNode::Split()
{
    int middleNumber = p_keys.size() / 2;
    std::vector<BTreeKey> rightArray;
    std::vector<std::shared_ptr<BTreeInnerNode>> rightInnerNodes;
    std::vector<std::shared_ptr<BTreeLeafNode>> rightLeafNodes;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        rightArray.push_back(p_keys[i]);
        p_keys.pop_back();

        if (m_hasLeafChildren)
        {
            rightLeafNodes.push_back(p_leafChildren[i]);
            p_leafChildren.pop_back();
        }
        else
        {
            rightInnerNodes.push_back(p_innerChildren[i]);
            p_innerChildren.pop_back();
        }
    }

    std::shared_ptr<BTreeInnerNode> rightNode = std::make_shared<BTreeInnerNode>(rightArray, rightInnerNodes, rightLeafNodes);
    if (p_father == nullptr)
        p_father = std::make_shared<BTreeInnerNode>(rightArray[0], this, rightNode);

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
        if (middleKey > key)
        {
            if (middle < lastItem && p_keys[middle + 1] <= key)
                return middle;

            min = middle + 1;
        }
        else if (middleKey < key)
        {
            if (middle > 0 && p_keys[middle - 1] >= key)
                return middle;

            max = middle - 1;
        }
        else
            return middle;
    }

    return lastItem + 1;
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

std::optional<Error> BTreeLeafNode::InsertOne(BTreeKey key, int value)
{
    p_keys.push_back(key);
    int nextPosition = 0;
    for (int i = p_keys.size() - 1; i > 0; i--)
    {
        if (p_keys[i] >= key)
        {
            p_keys[i] = p_keys[i - 1];
            nextPosition = i - 1;
        }
        else
            break;
    }

    p_keys[nextPosition] = key;

    // insert value here;
}

BTreeLeafNode::BTreeLeafNode(std::vector<BTreeKey> keys) : p_keys(keys)
{
}

std::shared_ptr<BTreeInnerNode> BTreeLeafNode::Split()
{
    int middleNumber = p_keys.size() / 2;
    std::vector<BTreeKey> rightArray;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        rightArray.push_back(p_keys[i]);
        p_keys.pop_back();
    }

    std::shared_ptr<BTreeLeafNode> rightNode = std::make_shared<BTreeLeafNode>(rightArray);
    if (p_father == nullptr)
        p_father = std::make_shared<BTreeInnerNode>(rightArray[0], this, rightNode);

    rightNode->p_father = p_father;

    return p_father;
}
