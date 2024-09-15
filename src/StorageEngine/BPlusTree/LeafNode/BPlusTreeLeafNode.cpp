#include "BPlusTreeLeafNode.hpp"

BPlusTreeLeafNode::BPlusTreeLeafNode(std::vector<BPlusTreeKey> keys) : p_keys(keys)
{
}

std::optional<int> BPlusTreeLeafNode::BinarySearchIndexData(BPlusTreeKey key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

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

    return std::nullopt;
}

BPlusTreeLeafNode::BPlusTreeLeafNode()
{
}

int BPlusTreeLeafNode::GetKeySize()
{
    return p_keys.size();
}

BPlusTreeKey BPlusTreeLeafNode::GetKey(int index)
{
    return p_keys[index];
}

BPlusTreeInnerNode *BPlusTreeLeafNode::GetFather()
{
    return p_father;
}

void BPlusTreeLeafNode::SetFather(BPlusTreeInnerNode *father)
{
    p_father = father;
}

BPlusTreeLeafNode *BPlusTreeLeafNode::GetNextPage()
{
    return p_nextPage;
}

BPlusTreeLeafNode *BPlusTreeLeafNode::GetPreviousPage()
{
    return p_previousPage;
}

std::optional<int> BPlusTreeLeafNode::FindOne(BPlusTreeKey key)
{
    auto indexOfItem = BinarySearchIndexData(key);
    if (!indexOfItem.has_value())
        return indexOfItem;

    // use here to access database and fetch data
    // for now, its just return the index
    return indexOfItem.value();
}

std::optional<Error> BPlusTreeLeafNode::InsertOne(BPlusTreeKey key, int value)
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

std::optional<Error> BPlusTreeLeafNode::DeleteOne(BPlusTreeKey key)
{
    auto indexPositionResult = BinarySearchIndexData(key);
    if (!indexPositionResult.has_value())
        return Error(ErrorType::NotFoundItem, "Item not found");

    auto index = indexPositionResult.value();

    p_keys.erase(p_keys.begin() + index);

    return std::nullopt;
}

BPlusTreeInnerNode *BPlusTreeLeafNode::Split()
{
    int middleNumber = p_keys.size() / 2;
    std::stack<BPlusTreeKey> stackKeys;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        stackKeys.push(p_keys[i]);
        p_keys.pop_back();
    }

    std::vector<BPlusTreeKey> rightArray;
    while (stackKeys.size() > 0)
    {
        rightArray.push_back(stackKeys.top());
        stackKeys.pop();
    }

    BPlusTreeLeafNode *rightNode = new BPlusTreeLeafNode(rightArray);
    if (p_father == nullptr)
        p_father = new BPlusTreeInnerNode(rightArray[0], this, rightNode);
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

std::optional<Error> BPlusTreeLeafNode::BorrowFromNextPage()
{
    BPlusTreeKey borrowedKey = p_nextPage->GetKey(0);
    p_keys.push_back(borrowedKey);
    p_nextPage->p_keys.erase(p_nextPage->p_keys.begin());

    auto fatherKeyIndexResult = p_father->GetKeyIndex(borrowedKey);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->UpdateKey(fatherKeyIndex, p_nextPage->GetKey(0));

    return std::nullopt;
}

std::optional<Error> BPlusTreeLeafNode::BorrowFromPreviousPage()
{
    p_keys.insert(p_keys.begin(), p_previousPage->GetKey(p_previousPage->GetKeySize() - 1));
    p_previousPage->p_keys.erase(p_previousPage->p_keys.end() - 1);

    auto fatherKeyIndexResult = p_father->GetLeafNodeIndex(this);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->UpdateKey(fatherKeyIndex - 1, p_keys[0]);

    return std::nullopt;
}

std::optional<Error> BPlusTreeLeafNode::MergeWithNextPage()
{
    p_keys.insert(p_keys.end(), p_nextPage->p_keys.begin(), p_nextPage->p_keys.end());

    p_nextPage = p_nextPage->p_nextPage;
    if (p_nextPage != nullptr)
        p_nextPage->p_previousPage = this;

    auto fatherKeyIndexResult = p_father->GetLeafNodeIndex(this);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->DeleteKeyByIndex(fatherKeyIndex);
    p_father->DeleteLeafChildrenByIndex(fatherKeyIndex + 1);

    return std::nullopt;
}