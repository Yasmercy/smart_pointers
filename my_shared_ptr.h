#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H

#include <vector>

template <typename T>
class SharedPtr {
public:
    // rule of 5
    SharedPtr();
    SharedPtr(T* ptr);
    SharedPtr(T* ptr, int* ref_count);
    SharedPtr(SharedPtr<T>& ptr);
    SharedPtr(SharedPtr<T>&& ptr);
    SharedPtr<T>& operator=(SharedPtr<T>& other);
    SharedPtr<T>& operator=(SharedPtr<T>&& other);
    ~SharedPtr();

    // useful methods
    T* get();
    void reset(T* ptr);
    T& operator*();
    T* operator->();
    int use_count();
    int* use_count_ptr();
    bool unique();

private:
    void increment_counter();
    void decrement_counter();
    void clear();

    T* ptr_;
    int* ref_count; // this should be an atomic int
};

// implementing shared_ptr
template <typename T>
SharedPtr<T>::SharedPtr() : ptr_(nullptr), ref_count(nullptr) {}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : ptr_(ptr), ref_count(new int {1}) {}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr, int* ref_count) : ptr_(ptr), ref_count(ref_count) {}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>& ptr) : ptr_(ptr.ptr_), ref_count(ptr.ref_count) {
    increment_counter();
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& ptr) : ptr_(ptr.ptr_), ref_count(ptr.ref_count) {
    // clear the other
    ptr.ptr_ = nullptr;
    ptr.ref_count = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>& other) {
    clear();
    ptr_ = other.ptr_;
    ref_count = other.ref_count;
    increment_counter();
    return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) {
    clear();
    ptr_ = other.ptr_;
    ref_count = other.ref_count;
    other.ptr_ = nullptr;
    other.ref_count = nullptr;
    return *this;
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    clear();
}

template <typename T>
T* SharedPtr<T>::get() {
    return ptr_;
}

template <typename T>
void SharedPtr<T>::reset(T* ptr) {
    clear();
    ref_count = new int {1};
    ptr_ = ptr;
}

template <typename T>
T& SharedPtr<T>::operator*() {
    if (ptr_ == nullptr) {
        throw (std::runtime_error("Dereferencing nullptr"));
    }
    return *ptr_;
}

template <typename T>
T* SharedPtr<T>::operator->() {
    if (ptr_ == nullptr) {
        throw (std::runtime_error("Dereferencing nullptr"));
    }
    return ptr_;
}

template <typename T>
int SharedPtr<T>::use_count() {
    if (ref_count == nullptr) {
        return -1;
    }
    return *ref_count;
}

template <typename T>
bool SharedPtr<T>::unique() {
    return use_count() == 1;
}

template <typename T>
void SharedPtr<T>::clear() {
    // decrement (maybe delete) the current object
    if (ptr_ != nullptr) {
        decrement_counter();
        if (*ref_count == 0) {
            delete ptr_;
            delete ref_count;
            ptr_ = nullptr;
            ref_count = nullptr;
        }
    }
}

template <typename T>
void SharedPtr<T>::decrement_counter() {
    --*ref_count;
}

template <typename T>
void SharedPtr<T>::increment_counter() {
    ++*ref_count;
}

template <typename T>
int* SharedPtr<T>::use_count_ptr() {
    return ref_count;
}

// implementing non-member functions
template <typename T>
bool operator==(SharedPtr<T>& x, SharedPtr<T>& y) {
    return x.get() == y.get();
}

#endif
