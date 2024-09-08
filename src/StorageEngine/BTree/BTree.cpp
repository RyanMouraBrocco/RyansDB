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

                next = nullptr;
            }
            else
                next = next->GetInnerNodeByIndex(keyIndex);
        }
    }

    return Error(ErrorType::NotFoundItem, "Item not found");
}

std::optional<Error> BTree::Delete(BTreeKey key)
{
    if (p_leafRoot)
        return DeleteLeaf(key);

    return DeleteInnerNode(key);
}

std::optional<Error> BTree::DeleteLeaf(BTreeKey key)
{
    auto deleteResult = p_leafRoot->DeleteOne(key);
    if (deleteResult.has_value())
        return deleteResult;

    return std::nullopt;
}

std::optional<Error> BTree::DeleteInnerNode(BTreeKey key)
{
    auto next = p_innerRoot;
    while (next != nullptr)
    {
        int keyIndex = next->BinarySearchIndexForNextNode(key);
        if (next->GetHasLeafChildren())
        {
            auto leafNode = next->GetLeafNodeByIndex(keyIndex);
            auto deleteResult = leafNode->DeleteOne(key);
            if (deleteResult.has_value())
                return deleteResult;

            auto fatherIndex = keyIndex > 0 ? keyIndex - 1 : 0;
            if (leafNode->GetKeySize() < MIN_TREE_CHILDREN)
            {
                auto right = leafNode->GetNextPage();
                auto left = leafNode->GetPreviousPage();

                if (right != nullptr &&
                    right->GetFather() == leafNode->GetFather() &&
                    right->GetKeySize() > MIN_TREE_CHILDREN)
                {
                    auto borrowedResult = leafNode->BorrowFromNextPage();
                    if (borrowedResult.has_value())
                        return borrowedResult;
                }
                else if (left != nullptr &&
                         left->GetFather() == leafNode->GetFather() &&
                         left->GetKeySize() > MIN_TREE_CHILDREN)
                {
                    auto borrowedResult = leafNode->BorrowFromPreviousPage();
                    if (borrowedResult.has_value())
                        return borrowedResult;
                }
                else if (right != nullptr &&
                         right->GetFather() == leafNode->GetFather() &&
                         right->GetKeySize() <= MIN_TREE_CHILDREN)
                {
                    auto mergedResult = leafNode->MergeWithNextPage();
                    if (mergedResult.has_value())
                        return mergedResult;
                }
                else if (left != nullptr &&
                         left->GetFather() == leafNode->GetFather() &&
                         left->GetKeySize() <= MIN_TREE_CHILDREN)
                {
                    auto mergedResult = left->MergeWithNextPage();
                    if (mergedResult.has_value())
                        return mergedResult;
                }
            }

            auto father = next;
            while (father != nullptr)
            {
                auto findKeyResult = father->GetKeyIndex(key);
                if (std::holds_alternative<int>(findKeyResult))
                {
                    int keyIndex = std::get<int>(findKeyResult);
                    auto innerNodePath = father;
                    if (!innerNodePath->GetHasLeafChildren())
                    {
                        auto innerNodePath = father->GetInnerNodeByIndex(keyIndex + 1);
                        while (!innerNodePath->GetHasLeafChildren())
                            innerNodePath = innerNodePath->GetInnerNodeByIndex(0);
                    }

                    father->UpdateKey(keyIndex, innerNodePath->GetLeafNodeByIndex(0)->GetKey(0));
                }

                if (father == p_innerRoot)
                {
                    if (father->GetKeySize() == 0)
                    {
                        if (father->GetHasLeafChildren())
                        {
                            p_leafRoot = father->GetLeafNodeByIndex(0);
                            p_leafRoot->SetFather(nullptr);
                            p_innerRoot = nullptr;
                        }
                        else
                        {
                            p_innerRoot = father->GetInnerNodeByIndex(0);
                            p_innerRoot->SetFather(nullptr);
                        }
                    }
                }
                else
                {
                    auto fatherOfFatherIndex = father->GetFather()->BinarySearchIndexForNextNode(key);
                    BTreeInnerNode *nextInnerNode = nullptr;
                    BTreeInnerNode *previousInnerNode = nullptr;

                    if (fatherOfFatherIndex < father->GetFather()->GetKeySize() + 2)
                        nextInnerNode = father->GetFather()->GetInnerNodeByIndex(fatherOfFatherIndex + 1);

                    if (fatherOfFatherIndex > 0)
                        previousInnerNode = father->GetFather()->GetInnerNodeByIndex(fatherOfFatherIndex - 1);

                    if (nextInnerNode != nullptr && nextInnerNode->GetKeySize() > MIN_TREE_CHILDREN)
                        father->BorrowFromRightNode(fatherOfFatherIndex, nextInnerNode);
                    else if (previousInnerNode != nullptr && previousInnerNode->GetKeySize() > MIN_TREE_CHILDREN)
                        father->BorrowFromRightNode(fatherOfFatherIndex, previousInnerNode);
                    else if (nextInnerNode != nullptr && nextInnerNode->GetKeySize() <= MIN_TREE_CHILDREN)
                        father->MergeWithRightNode(fatherOfFatherIndex, nextInnerNode);
                    else if (previousInnerNode != nullptr && previousInnerNode->GetKeySize() <= MIN_TREE_CHILDREN)
                        previousInnerNode->MergeWithRightNode(fatherOfFatherIndex - 1, father);
                }

                father = father->GetFather();
            }

            next = nullptr;
        }
        else
            next = next->GetInnerNodeByIndex(keyIndex);
    }

    return std::nullopt;
}

std::vector<BTreeKey> BTree::ListAll()
{
    std::vector<BTreeKey> keys;

    if (m_isLeaf)
    {
        for (int i = 0; i < p_leafRoot->GetKeySize(); i++)
        {
            keys.push_back(p_leafRoot->GetKey(i));
        }
    }
    else
    {
        auto next = p_innerRoot;
        while (next != nullptr)
        {
            if (next->GetHasLeafChildren())
            {
                auto leaf = next->GetLeafNodeByIndex(0);
                while (leaf != nullptr)
                {
                    for (int i = 0; i < leaf->GetKeySize(); i++)
                    {
                        keys.push_back(leaf->GetKey(i));
                    }

                    leaf = leaf->GetNextPage();
                }

                next = nullptr;
            }
            else
                next = next->GetInnerNodeByIndex(0);
        }
    }

    return keys;
}

BTreeInnerNode::BTreeInnerNode()
{
}

BTreeInnerNode::BTreeInnerNode(std::vector<BTreeKey> keys, std::vector<BTreeInnerNode *> innerNodes, std::vector<BTreeLeafNode *> leafNodes) : p_keys(keys), p_innerChildren(innerNodes), p_leafChildren(leafNodes)
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

    std::vector<BTreeKey> rightArray;
    std::vector<BTreeLeafNode *> rightLeafNodes;
    std::vector<BTreeInnerNode *> rightInnerNodes;

    BTreeKey toUpValue = stackKeys.top();
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

    BTreeInnerNode *rightNode = new BTreeInnerNode(rightArray, rightInnerNodes, rightLeafNodes);
    if (p_father == nullptr)
        p_father = new BTreeInnerNode(toUpValue, this, rightNode);
    else
        p_father->InsertOne(toUpValue, rightNode);

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

void BTreeInnerNode::SetFather(BTreeInnerNode *father)
{
    p_father = father;
}

void BTreeInnerNode::BorrowFromRightNode(int currentFatherPosition, BTreeInnerNode *rightNode)
{
    p_keys.push_back(p_father->GetKey(currentFatherPosition));
    p_father->UpdateKey(currentFatherPosition, rightNode->GetKey(0));
    rightNode->DeleteKeyByIndex(0);

    if (m_hasLeafChildren)
    {
        p_innerChildren.push_back(rightNode->p_innerChildren[0]);
        rightNode->p_innerChildren[0]->SetFather(this);
        rightNode->DeleteInnerChildrenByIndex(0);
    }
    else
    {
        p_leafChildren.push_back(rightNode->p_leafChildren[0]);
        rightNode->p_leafChildren[0]->SetFather(this);
        rightNode->DeleteLeafChildrenByIndex(0);
    }
}

void BTreeInnerNode::BorrowFromLeftNode(int currentFatherPosition, BTreeInnerNode *leftNode)
{
    p_keys.insert(p_keys.begin(), p_father->GetKey(currentFatherPosition - 1));
    p_father->UpdateKey(currentFatherPosition - 1, leftNode->GetKey(leftNode->GetKeySize() - 1));
    leftNode->DeleteKeyByIndex(leftNode->GetKeySize() - 1);

    if (m_hasLeafChildren)
    {
        p_innerChildren.insert(p_innerChildren.begin(), leftNode->p_innerChildren[leftNode->GetInnerNodeSize() - 1]);
        p_innerChildren[0]->SetFather(this);
        leftNode->DeleteInnerChildrenByIndex(leftNode->GetInnerNodeSize() - 1);
    }
    else
    {
        p_leafChildren.insert(p_leafChildren.begin(), leftNode->p_leafChildren[leftNode->GetLeafNodeSize() - 1]);
        p_leafChildren[0]->SetFather(this);
        leftNode->DeleteLeafChildrenByIndex(leftNode->GetLeafNodeSize() - 1);
    }
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

BTreeKey BTreeInnerNode::GetKey(int index)
{
    return p_keys[index];
}

void BTreeInnerNode::UpdateKey(int index, BTreeKey key)
{
    p_keys[index] = key;
}

BTreeInnerNode *BTreeInnerNode::GetFather()
{
    return p_father;
}

std::variant<int, Error> BTreeInnerNode::GetKeyIndex(BTreeKey key)
{
    int min = 0;
    int max = p_keys.size() - 1;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BTreeKey middleKey = p_keys[middle];
        if (key > middleKey)
            min = middle + 1;
        else if (key < middleKey)
            max = middle - 1;
        else
            return middle;
    }

    return Error(ErrorType::NotFoundItem, "It was not possible to found this key");
}

void BTreeInnerNode::DeleteKeyByIndex(int index)
{
    p_keys.erase(p_keys.begin() + index);
}

void BTreeInnerNode::DeleteLeafChildrenByIndex(int index)
{
    p_leafChildren.erase(p_leafChildren.begin() + index);
}

void BTreeInnerNode::DeleteInnerChildrenByIndex(int index)
{
    p_innerChildren.erase(p_innerChildren.begin() + index);
}

int BTreeInnerNode::GetInnerNodeSize()
{
    p_innerChildren.size();
}

int BTreeInnerNode::GetLeafNodeSize()
{
    p_leafChildren.size();
}

void BTreeInnerNode::MergeWithRightNode(int currentFatherPositon, BTreeInnerNode *rightNode)
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
        if (key > middleKey)
            min = middle + 1;
        else if (key < middleKey)
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

    if (p_nextPage != nullptr)
    {
        rightNode->p_nextPage = p_nextPage;
        p_nextPage->p_previousPage = rightNode;
    }

    p_nextPage = rightNode;
    rightNode->p_father = p_father;
    rightNode->p_previousPage = this;

    return p_father;
}

void BTreeLeafNode::SetFather(BTreeInnerNode *father)
{
    p_father = father;
}

std::optional<Error> BTreeLeafNode::DeleteOne(BTreeKey key)
{
    auto indexPositionResult = BinarySearchIndexData(key);
    if (!indexPositionResult.has_value())
        return Error(ErrorType::NotFoundItem, "Item not found");

    auto index = indexPositionResult.value();

    p_keys.erase(p_keys.begin() + index);

    return std::nullopt;
}

BTreeKey BTreeLeafNode::GetKey(int index)
{
    return p_keys[index];
}

BTreeLeafNode *BTreeLeafNode::GetNextPage()
{
    return p_nextPage;
}

BTreeLeafNode *BTreeLeafNode::GetPreviousPage()
{
    return p_previousPage;
}

BTreeInnerNode *BTreeLeafNode::GetFather()
{
    return p_father;
}

std::optional<Error> BTreeLeafNode::BorrowFromNextPage()
{
    BTreeKey borrowedKey = p_nextPage->GetKey(0);
    p_keys.push_back(borrowedKey);
    p_nextPage->p_keys.erase(p_nextPage->p_keys.begin());

    auto fatherKeyIndexResult = p_father->GetKeyIndex(borrowedKey);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->UpdateKey(fatherKeyIndex, p_nextPage->GetKey(0));

    return std::nullopt;
}

std::optional<Error> BTreeLeafNode::BorrowFromPreviousPage()
{
    BTreeKey borrowedKey = p_previousPage->GetKey(p_previousPage->GetKeySize() - 1);
    p_keys.insert(p_keys.begin(), borrowedKey);
    p_previousPage->p_keys.erase(p_previousPage->p_keys.end() - 1);

    auto fatherKeyIndexResult = p_father->GetKeyIndex(borrowedKey);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->UpdateKey(fatherKeyIndex, p_keys[0]);

    return std::nullopt;
}

std::optional<Error> BTreeLeafNode::MergeWithNextPage()
{
    auto firstNextPageKey = p_nextPage->GetKey(0);
    p_keys.insert(p_keys.end(), p_nextPage->p_keys.begin(), p_nextPage->p_keys.end());

    p_nextPage = p_nextPage->p_nextPage;
    if (p_nextPage != nullptr)
        p_nextPage->p_previousPage = this;

    auto fatherKeyIndexResult = p_father->GetKeyIndex(firstNextPageKey);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->DeleteKeyByIndex(fatherKeyIndex);
    p_father->DeleteLeafChildrenByIndex(fatherKeyIndex + 1);

    return std::nullopt;
}