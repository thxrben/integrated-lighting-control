#ifndef LINKED_LIST_ITERATOR_HPP
#define LINKED_LIST_ITERATOR_HPP

#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "linked_list.hpp"
#include "linked_list_node.hpp"

template <typename T>
class LinkedListIterator
{
public:
    LinkedListIterator(std::shared_ptr<LinkedListNode<T>> start,
                                              std::shared_ptr<LinkedListNode<T>> end)
    {
        this->current = start;
        this->end = end;
    };

    bool hasNext()
    {
        return this->current != nullptr;
    };

    std::shared_ptr<T> next()
    {
        if(!this->hasNext())
            return nullptr;

        std::shared_ptr<T> obj = this->current->obj;
        this->current = this->current->next;
        return obj;
    };

private:
    std::shared_ptr<LinkedListNode<T>> current;
    std::shared_ptr<LinkedListNode<T>> end;
};

#endif