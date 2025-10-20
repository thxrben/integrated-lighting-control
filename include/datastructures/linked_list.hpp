#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <stdbool.h>

#include "linked_list_node.hpp"
#include "linked_list_iterator.hpp"

template <typename T>
class LinkedList
{
private:
    std::shared_ptr<LinkedListNode<T>> start;
    std::shared_ptr<LinkedListNode<T>> end;

public:
    LinkedList()
    {
        this->start = nullptr;
        this->end = nullptr;
    };

    void insert(std::shared_ptr<T> obj)
    {
        this->start = std::make_shared<LinkedListNode<T>>(obj, this->start);

        if (this->end == nullptr || this->end.get() == nullptr)
            this->end = this->start;
    }

    void insert(std::shared_ptr<T> obj, int pos)
    {
        if (this->isEmpty() || pos <= 0)
        {
            this->insert(obj);
            return;
        }

        LinkedListNode<T> *current = this->start.get();

        std::shared_ptr<LinkedListNode<T>> newNode = std::make_shared<LinkedListNode<T>>(obj, current->next);

        while ((--pos > 0) && (current->next != nullptr))
            current = current->next.get();

        current->next = newNode;

        if (current->next == this->end)
            this->end = newNode;
    }

    void append(std::shared_ptr<T> obj)
    {
        std::shared_ptr<LinkedListNode<T>> newNode = std::make_shared<LinkedListNode<T>>(obj, nullptr);

        if (this->isEmpty())
        {
            this->start = std::make_shared<LinkedListNode<T>>(obj, nullptr);
            this->end = this->start;
        }
        else
        {
            this->end->next = newNode;
            this->end = newNode;
        }
    }

    bool isEmpty()
    {
        return this->start == nullptr && this->end == nullptr;
    }

    std::unique_ptr<LinkedListIterator<T>> iterator()
    {
        return std::make_unique<LinkedListIterator<T>>(this->start, this->end);
    }

    void remove(std::shared_ptr<T> obj);
    int search(std::shared_ptr<T> obj);
};

#endif