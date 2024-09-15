#pragma once

#include <vector>
#include <optional>
#include <stack>
#include "../Key/BPlusTreeKey.hpp"
#include "../../../Error/error.hpp"
#include "../InnerNode/BPlusTreeInnerNode.hpp"

template <typename Key>
class BPlusTreeInnerNode;

template <typename Key>
class BPlusTreeLeafNode
{
private:
    BPlusTreeInnerNode<Key> *p_father = nullptr;
    BPlusTreeLeafNode<Key> *p_nextPage = nullptr;
    BPlusTreeLeafNode<Key> *p_previousPage = nullptr;
    std::vector<BPlusTreeKey<Key>> p_keys;
    // here we could receive the factory to see what it will be saved on leaf page

    BPlusTreeLeafNode(std::vector<BPlusTreeKey<Key>> initKeys);
    std::optional<int> BinarySearchIndexData(BPlusTreeKey<Key> key);

public:
    BPlusTreeLeafNode();

    // GETTERS AND SETTERS
    int GetKeySize();
    BPlusTreeKey<Key> GetKey(int index);
    BPlusTreeInnerNode<Key> *GetFather();
    void SetFather(BPlusTreeInnerNode<Key> *father);
    BPlusTreeLeafNode<Key> *GetNextPage();
    BPlusTreeLeafNode<Key> *GetPreviousPage();
    // GETTERS AND SETTERS

    std::optional<int> FindOne(BPlusTreeKey<Key> key);
    std::optional<Error> InsertOne(BPlusTreeKey<Key> key, int value);
    std::optional<Error> DeleteOne(BPlusTreeKey<Key> key);
    BPlusTreeInnerNode<Key> *Split();
    std::optional<Error> BorrowFromNextPage();
    std::optional<Error> BorrowFromPreviousPage();
    std::optional<Error> MergeWithNextPage();

    BPlusTreeLeafNode(const BPlusTreeLeafNode &) = delete;
    BPlusTreeLeafNode(BPlusTreeLeafNode &&) = delete;
    BPlusTreeLeafNode &operator=(const BPlusTreeLeafNode &) = delete;
    BPlusTreeLeafNode &operator=(BPlusTreeLeafNode &&) = delete;
};

template <typename Key>
BPlusTreeLeafNode<Key>::BPlusTreeLeafNode(std::vector<BPlusTreeKey<Key>> keys) : p_keys(keys)
{
}

template <typename Key>
std::optional<int> BPlusTreeLeafNode<Key>::BinarySearchIndexData(BPlusTreeKey<Key> key)
{
    int lastItem = p_keys.size() - 1;
    int min = 0;
    int max = lastItem;

    while (min <= max)
    {
        int middle = min + (max - min) / 2;
        BPlusTreeKey<Key> middleKey = p_keys[middle];
        if (key > middleKey)
            min = middle + 1;
        else if (key < middleKey)
            max = middle - 1;
        else
            return middle;
    }

    return std::nullopt;
}

template <typename Key>
BPlusTreeLeafNode<Key>::BPlusTreeLeafNode()
{
}

template <typename Key>
int BPlusTreeLeafNode<Key>::GetKeySize()
{
    return p_keys.size();
}

template <typename Key>
BPlusTreeKey<Key> BPlusTreeLeafNode<Key>::GetKey(int index)
{
    return p_keys[index];
}

template <typename Key>
BPlusTreeInnerNode<Key> *BPlusTreeLeafNode<Key>::GetFather()
{
    return p_father;
}

template <typename Key>
void BPlusTreeLeafNode<Key>::SetFather(BPlusTreeInnerNode<Key> *father)
{
    p_father = father;
}

template <typename Key>
BPlusTreeLeafNode<Key> *BPlusTreeLeafNode<Key>::GetNextPage()
{
    return p_nextPage;
}

template <typename Key>
BPlusTreeLeafNode<Key> *BPlusTreeLeafNode<Key>::GetPreviousPage()
{
    return p_previousPage;
}

template <typename Key>
std::optional<int> BPlusTreeLeafNode<Key>::FindOne(BPlusTreeKey<Key> key)
{
    auto indexOfItem = BinarySearchIndexData(key);
    if (!indexOfItem.has_value())
        return indexOfItem;

    // use here to access database and fetch data
    // for now, its just return the index
    return indexOfItem.value();
}

template <typename Key>
std::optional<Error> BPlusTreeLeafNode<Key>::InsertOne(BPlusTreeKey<Key> key, int value)
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

template <typename Key>
std::optional<Error> BPlusTreeLeafNode<Key>::DeleteOne(BPlusTreeKey<Key> key)
{
    auto indexPositionResult = BinarySearchIndexData(key);
    if (!indexPositionResult.has_value())
        return Error(ErrorType::NotFoundItem, "Item not found");

    auto index = indexPositionResult.value();

    p_keys.erase(p_keys.begin() + index);

    return std::nullopt;
}

template <typename Key>
BPlusTreeInnerNode<Key> *BPlusTreeLeafNode<Key>::Split()
{
    int middleNumber = p_keys.size() / 2;
    std::stack<BPlusTreeKey<Key>> stackKeys;
    for (int i = p_keys.size() - 1; i >= middleNumber; i--)
    {
        stackKeys.push(p_keys[i]);
        p_keys.pop_back();
    }

    std::vector<BPlusTreeKey<Key>> rightArray;
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

template <typename Key>
std::optional<Error> BPlusTreeLeafNode<Key>::BorrowFromNextPage()
{
    BPlusTreeKey<Key> borrowedKey = p_nextPage->GetKey(0);
    p_keys.push_back(borrowedKey);
    p_nextPage->p_keys.erase(p_nextPage->p_keys.begin());

    auto fatherKeyIndexResult = p_father->GetKeyIndex(borrowedKey);
    if (std::holds_alternative<Error>(fatherKeyIndexResult))
        return std::get<Error>(fatherKeyIndexResult);

    auto fatherKeyIndex = std::get<int>(fatherKeyIndexResult);

    p_father->UpdateKey(fatherKeyIndex, p_nextPage->GetKey(0));

    return std::nullopt;
}

template <typename Key>
std::optional<Error> BPlusTreeLeafNode<Key>::BorrowFromPreviousPage()
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

template <typename Key>
std::optional<Error> BPlusTreeLeafNode<Key>::MergeWithNextPage()
{
    auto deletedPage = p_nextPage;
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

    delete deletedPage;

    return std::nullopt;
}