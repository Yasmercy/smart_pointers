#include <iostream>
#include <utility>
#include "my_unique_ptr.h"
#include "my_shared_ptr.h"
#include "my_weak_ptr.h"
#include "circular_reference.h"

void test_unique_ptr() {
    UniquePtr<int> p {new int {3}};
    int data = *p;
    std::cout << data << "=3\n";

    UniquePtr<int> p1 {new int {4}};
    p = p1;
    p1.reset(new int {5}); 
    std::cout << *p << "=4\n";
    std::cout << *p1 << "=5\n";

    UniquePtr<int> p2 {new int {6}};
    p1 = std::move(p2);
    std::cout << *p << "=4\n";
    std::cout << *p1 << "=6\n";

    try {
        std::cout << *p2 << " should error\n";
    } catch (std::runtime_error& e) {
        std::cout << "Correctly errored: " << e.what() << "\n";
    }

    std::swap(p, p1);
    std::cout << *p << "=6\n";
    std::cout << *p1 << "=4\n";
}

void test_shared_ptr() {
    SharedPtr<int> p {new int {5}};
    std::cout << *p << "=5\n";
    std::cout << p.use_count() << "=1\n";
    SharedPtr<int> p1 {new int {6}};
    std::cout << *p1 << "=6\n";
    std::cout << p1.use_count() << "=1\n";
    std::cout << (p.unique() && p1.unique()) << "=1\n";

    SharedPtr<int> p2{p1};
    std::cout << (p1 == p2) << "=1\n";
    std::cout << p1.use_count() << "=2\n";
    std::cout << p2.use_count() << "=2\n";
    std::cout << *p2 << "=6\n";

    SharedPtr<int> p3 = p;
    p2 = std::move(p);
    std::cout << p.use_count() << "=-1\n";
    std::cout << p2.use_count() << "=2\n";

    p2.reset(new int {7});
    std::cout << p2.use_count() << "=1\n";
    std::cout << p1.use_count() << "=1\n";
    std::cout << *p2 << "=7\n";
}

void test_expired_weak_ptr() {
    SharedPtr<int> p1 {new int {1}};
    WeakPtr<int> w {p1};

    std::cout << p1.use_count() << "=1\n";
    p1.reset();
    std::cout << p1.use_count() << "=1\n";
    SharedPtr<int> p2 = w.upgrade(); // this would not work

    try {
        int val = *p2;
        std::cout << "p2 = " << val << "\n";
    } 
    catch (std::runtime_error e) {
        std::cout << "no dangling pointer created!" << "\n";
        std::cout << e.what() << "\n";
    }
}

int main() {
    // test_unique_ptr();
    // test_shared_ptr();
    // shared_ptr_leak();
    weak_ptr_no_leak();
    // test_expired_weak_ptr();
}
