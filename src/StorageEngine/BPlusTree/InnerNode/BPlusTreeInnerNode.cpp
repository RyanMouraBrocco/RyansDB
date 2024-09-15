#include "BPlusTreeInnerNode.hpp"

BPlusTreeInnerNode::BPlusTreeInnerNode(std::vector<BPlusTreeKey> keys, std::vector<BPlusTreeInnerNode *> innerNodes, std::vector<BPlusTreeLeafNode *> leafNodes) : p_keys(keys), p_innerChildren(innerNodes), p_leafChildren(leafNodes)
{
    m_hasLeafChildren = leafNodes.size() > 0;

    for (int i = 0; i < p_leafChildren.size(); i++)
    {
        p_leafChildren[i]->SetFather(this);
    }

    for (int i = 0; i < p_innerChildren.size(); i++)
    {
        p_innerChildren[i]->p_father = this;
    }
}

BPlusTreeInnerNode::BPlusTreeInnerNode(BPlusTreeKey key, BPlusTreeInnerNode *leftNode, BPlusTreeInnerNode *rightNode)
{
    p_keys.push_back(key);
    p_innerChildren.push_back(leftNode);
    p_innerChildren.push_back(rightNode);
    m_hasLeafChildren = false;
}

BPlusTreeKey BPlusTreeInnerNode::GetKey(int index)
{
    return p_keys[index];
}

int BPlusTreeInnerNode::GetLeafNodeSize()
{
    return p_leafChildren.size();
}

void BPlusTreeInnerNode::DeleteInnerChildrenByIndex(int index)
{
    p_innerChildren.erase(p_innerChildren.begin() + index);
}

void BPlusTreeInnerNode::InsertOne(BPlusTreeKey key, BPlusTreeInnerNode *rightNode)
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

BPlusTreeInnerNode::BPlusTreeInnerNode(BPlusTreeKey key, BPlusTreeLeafNode *leftNode, BPlusTreeLeafNode *rightNode)
{
    p_keys.push_back(key);
    p_leafChildren.push_back(leftNode);
    p_leafChildren.push_back(rightNode);
    m_hasLeafChildren = true;
}

bool BPlusTreeInnerNode::GetHasLeafChildren()
{
    return m_hasLeafChildren;
}

int BPlusTreeInnerNode::GetKeySize()
{
    return p_keys.size();
}

void BPlusTreeInnerNode::DeleteKeyByIndex(int index)
{
    p_keys.erase(p_keys.begin() + index);
}

void BPlusTreeInnerNode::UpdateKey(int index, BPlusTreeKey key)
{
    p_keys[index] = key;
}

std::variant<int, Error> BPlusTreeInnerNode::GetKeyIndex(BPlusTreeKey key)
{
    int min = 0;
    int max = p_keys.size() - 1;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BPlusTreeKey middleKey = p_keys[middle];
        if (key > middleKey)
            min = middle + 1;
        else if (key < middleKey)
            max = middle - 1;
        else
            return middle;
    }

    return Error(ErrorType::NotFoundItem, "It was not possible to found this key");
}

BPlusTreeInnerNode *BPlusTreeInnerNode::GetFather()
{
    return p_father;
}

void BPlusTreeInnerNode::SetFather(BPlusTreeInnerNode *father)
{
    p_father = father;
}

int BPlusTreeInnerNode::GetInnerNodeSize()
{
    return p_innerChildren.size();
}

std::variant<int, Error> BPlusTreeInnerNode::GetInnerNodeIndex(BPlusTreeInnerNode *innerNode)
{
    for (int i = 0; i < p_innerChildren.size(); i++)
    {
        if (p_innerChildren[i] == innerNode)
            return i;
    }

    return Error(ErrorType::NotFoundItem, "It was not possible to found this key");
}

BPlusTreeInnerNode *BPlusTreeInnerNode::GetInnerNodeByIndex(int index)
{
    return p_innerChildren[index];
}

std::variant<int, Error> BPlusTreeInnerNode::GetLeafNodeIndex(BPlusTreeLeafNode *leafNode)
{
    for (int i = 0; i < p_leafChildren.size(); i++)
    {
        if (p_leafChildren[i] == leafNode)
            return i;
    }

    return Error(ErrorType::NotFoundItem, "It was not possible to found this key");
}

BPlusTreeLeafNode *BPlusTreeInnerNode::GetLeafNodeByIndex(int index)
{
    return p_leafChildren[index];
}

void BPlusTreeInnerNode::DeleteLeafChildrenByIndex(int index)
{
    p_leafChildren.erase(p_leafChildren.begin() + index);
}

int BPlusTreeInnerNode::BinarySearchIndexForNextNode(BPlusTreeKey key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BPlusTreeKey middleKey = p_keys[middle];
        if (key >= middleKey)
        {
            if (middle < lastItem && key < p_keys[middle + 1])
                return middle + 1;

            min = middle + 1;
        }
        else
        {
            if (middle > 0 && key >= p_keys[middle - 1])
                return middle - 1;

            max = middle - 1;
        }
    }

    return key < p_keys[0] ? 0 : lastItem + 1;
}

BPlusTreeInnerNode *BPlusTreeInnerNode::Split()
{
    int middleNumber = p_keys.size() / 2;

    std::stack<BPlusTreeKey> stackKeys;
    std::stack<BPlusTreeLeafNode *> stackLeafNodes;
    std::stack<BPlusTreeInnerNode *> stackInnerNodes;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        stackKeys.push(p_keys[i]);
        p_keys.pop_back();
    }

    if (m_hasLeafChildren)
    {
        for (int i = p_leafChildren.size() - 1; i > middleNumber; i--)
        {
            stackLeafNodes.push(p_leafChildren[i]);
            p_leafChildren.pop_back();
        }
    }
    else
    {
        for (int i = p_innerChildren.size() - 1; i > middleNumber; i--)
        {
            stackInnerNodes.push(p_innerChildren[i]);
            p_innerChildren.pop_back();
        }
    }

    std::vector<BPlusTreeKey> rightArray;
    std::vector<BPlusTreeLeafNode *> rightLeafNodes;
    std::vector<BPlusTreeInnerNode *> rightInnerNodes;

    BPlusTreeKey toUpValue = stackKeys.top();
    stackKeys.pop();

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

    BPlusTreeInnerNode *rightNode = new BPlusTreeInnerNode(rightArray, rightInnerNodes, rightLeafNodes);
    if (p_father == nullptr)
        p_father = new BPlusTreeInnerNode(toUpValue, this, rightNode);
    else
        p_father->InsertOne(toUpValue, rightNode);

    rightNode->p_father = p_father;

    return p_father;
}

void BPlusTreeInnerNode::InsertOne(BPlusTreeKey key, BPlusTreeLeafNode *rightNode)
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

void BPlusTreeInnerNode::BorrowFromRightNode(int currentFatherPosition, BPlusTreeInnerNode *rightNode)
{
    p_keys.push_back(p_father->GetKey(currentFatherPosition));
    p_father->UpdateKey(currentFatherPosition, rightNode->GetKey(0));
    rightNode->DeleteKeyByIndex(0);

    if (m_hasLeafChildren)
    {
        auto borrowedNode = rightNode->GetLeafNodeByIndex(0);
        p_leafChildren.push_back(borrowedNode);
        borrowedNode->SetFather(this);
        rightNode->DeleteLeafChildrenByIndex(0);
    }
    else
    {
        auto borrowedNode = rightNode->GetInnerNodeByIndex(0);
        p_innerChildren.push_back(borrowedNode);
        borrowedNode->SetFather(this);
        rightNode->DeleteInnerChildrenByIndex(0);
    }
}

void BPlusTreeInnerNode::BorrowFromLeftNode(int currentFatherPosition, BPlusTreeInnerNode *leftNode)
{
    p_keys.insert(p_keys.begin(), p_father->GetKey(currentFatherPosition - 1));
    p_father->UpdateKey(currentFatherPosition - 1, leftNode->GetKey(leftNode->GetKeySize() - 1));
    leftNode->DeleteKeyByIndex(leftNode->GetKeySize() - 1);

    if (m_hasLeafChildren)
    {
        p_leafChildren.insert(p_leafChildren.begin(), leftNode->p_leafChildren[leftNode->GetLeafNodeSize() - 1]);
        p_leafChildren[0]->SetFather(this);
        leftNode->DeleteLeafChildrenByIndex(leftNode->GetLeafNodeSize() - 1);
    }
    else
    {
        p_innerChildren.insert(p_innerChildren.begin(), leftNode->p_innerChildren[leftNode->GetInnerNodeSize() - 1]);
        p_innerChildren[0]->SetFather(this);
        leftNode->DeleteInnerChildrenByIndex(leftNode->GetInnerNodeSize() - 1);
    }
}

void BPlusTreeInnerNode::MergeWithRightNode(int currentFatherPositon, BPlusTreeInnerNode *rightNode)
{
    p_keys.push_back(p_father->p_keys[currentFatherPositon]);
    p_father->DeleteKeyByIndex(currentFatherPositon);
    p_father->DeleteInnerChildrenByIndex(currentFatherPositon + 1);
    for (int i = 0; i < rightNode->GetKeySize(); i++)
        p_keys.push_back(rightNode->GetKey(i));

    if (m_hasLeafChildren)
    {
        for (int i = 0; i < rightNode->GetLeafNodeSize(); i++)
        {
            auto leafNode = rightNode->GetLeafNodeByIndex(i);
            leafNode->SetFather(this);
            p_leafChildren.push_back(leafNode);
        }
    }
    else
    {
        for (int i = 0; i < rightNode->GetInnerNodeSize(); i++)
        {
            auto innerNode = rightNode->GetInnerNodeByIndex(i);
            innerNode->SetFather(this);
            p_innerChildren.push_back(innerNode);
        }
    }
}