#include "BPlusTree.hpp"

BPlusTree::BPlusTree()
{
    m_isLeaf = true;
    p_innerRoot = nullptr;
    p_leafRoot = new BPlusTreeLeafNode();
}

std::optional<Error> BPlusTree::Insert(BPlusTreeKey key, int value)
{
    if (m_isLeaf)
        return InsertLeaf(key, value);

    return InsertInnerNode(key, value);
}

std::optional<Error> BPlusTree::InsertLeaf(BPlusTreeKey key, int value)
{
    auto insertResult = p_leafRoot->InsertOne(key, value);
    if (insertResult.has_value())
        return insertResult;

    if (p_leafRoot->GetKeySize() == MAX_TREE_CHILDREN)
    {
        BPlusTreeInnerNode *innerNode = p_leafRoot->Split();
        p_leafRoot = nullptr;
        p_innerRoot = innerNode;
        m_isLeaf = false;
    }

    return std::nullopt;
}

std::optional<Error> BPlusTree::InsertInnerNode(BPlusTreeKey key, int value)
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

std::variant<int, Error> BPlusTree::FindOne(BPlusTreeKey key)
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

std::optional<Error> BPlusTree::Delete(BPlusTreeKey key)
{
    if (p_leafRoot)
        return DeleteLeaf(key);

    return DeleteInnerNode(key);
}

std::optional<Error> BPlusTree::DeleteLeaf(BPlusTreeKey key)
{
    auto deleteResult = p_leafRoot->DeleteOne(key);
    if (deleteResult.has_value())
        return deleteResult;

    return std::nullopt;
}

std::optional<Error> BPlusTree::DeleteInnerNode(BPlusTreeKey key)
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
                if (father->GetKeySize() < MIN_TREE_CHILDREN)
                {
                    auto findKeyResult = father->GetKeyIndex(key);
                    if (std::holds_alternative<int>(findKeyResult))
                    {
                        int keyIndex = std::get<int>(findKeyResult);
                        auto innerNodePath = father;
                        if (!innerNodePath->GetHasLeafChildren())
                        {
                            innerNodePath = father->GetInnerNodeByIndex(keyIndex + 1);
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
                                m_isLeaf = true;
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
                        auto fatherOfFatherIndex = std::get<int>(father->GetFather()->GetInnerNodeIndex(father));
                        BPlusTreeInnerNode *nextInnerNode = nullptr;
                        BPlusTreeInnerNode *previousInnerNode = nullptr;

                        if (fatherOfFatherIndex + 1 < father->GetFather()->GetInnerNodeSize())
                            nextInnerNode = father->GetFather()->GetInnerNodeByIndex(fatherOfFatherIndex + 1);

                        if (fatherOfFatherIndex > 0)
                            previousInnerNode = father->GetFather()->GetInnerNodeByIndex(fatherOfFatherIndex - 1);

                        if (nextInnerNode != nullptr && nextInnerNode->GetKeySize() > MIN_TREE_CHILDREN)
                            father->BorrowFromRightNode(fatherOfFatherIndex, nextInnerNode);
                        else if (previousInnerNode != nullptr && previousInnerNode->GetKeySize() > MIN_TREE_CHILDREN)
                            father->BorrowFromLeftNode(fatherOfFatherIndex, previousInnerNode);
                        else if (nextInnerNode != nullptr && nextInnerNode->GetKeySize() <= MIN_TREE_CHILDREN)
                            father->MergeWithRightNode(fatherOfFatherIndex, nextInnerNode);
                        else if (previousInnerNode != nullptr && previousInnerNode->GetKeySize() <= MIN_TREE_CHILDREN)
                            previousInnerNode->MergeWithRightNode(fatherOfFatherIndex - 1, father);
                    }
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

std::vector<BPlusTreeKey> BPlusTree::ListAll()
{
    std::vector<BPlusTreeKey> keys;

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

