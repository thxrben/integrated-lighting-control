#ifndef LINKED_LIST_NODE_HPP
#define LINKED_LIST_NODE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <memory>

template<typename T>
struct LinkedListNode {
    
    std::shared_ptr<T> obj;

    std::shared_ptr<LinkedListNode<T>> next;
    std::shared_ptr<LinkedListNode<T>> prev;

    LinkedListNode() {
        this->obj = nullptr;
        this->next = nullptr;
        this->prev = nullptr;
    }

    LinkedListNode(std::shared_ptr<T> obj, std::shared_ptr<LinkedListNode<T>> next) {
        this->obj = obj;
        this->next = next;
        this->prev = nullptr;
    }
    
};

#endif