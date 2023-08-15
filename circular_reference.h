#ifndef CIRCULAR_REFERENCE_H
#define CIRCULAR_REFERENCE_H

#include "my_shared_ptr.h"

struct SharedNode {
    int data;
    SharedPtr<SharedNode> next;
};

void shared_ptr_leak() {
    SharedPtr<SharedNode> a {new SharedNode {1, SharedPtr<SharedNode> {}}};
    SharedPtr<SharedNode> b {new SharedNode {2, SharedPtr<SharedNode> {}}};
    a->next = b;
    b->next = a;

    std::cout << a->data << "\n";
    std::cout << a->next->data << "\n";
    std::cout << a->next->next->data << "\n";
}

struct WeakNode {
    int data;
    WeakPtr<WeakNode> next;
};

void weak_ptr_no_leak() {
    SharedPtr<WeakNode> a {new WeakNode {3, WeakPtr<WeakNode>()}};
    SharedPtr<WeakNode> b {new WeakNode {4, WeakPtr<WeakNode>()}};
    a->next = WeakPtr{b};
    b->next = WeakPtr{a};
    
    std::cout << a->data << "\n";
    std::cout << a->next.upgrade()->data << "\n";
    std::cout << a->next.upgrade()->next.upgrade()->data << "\n";
}

#endif
