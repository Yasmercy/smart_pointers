#ifndef MY_WEAK_PTR_H
#define MY_WEAK_PTR_H

#include "my_shared_ptr.h"

template <typename T>
class WeakPtr {
public:
    // rule of 5
    WeakPtr();
    WeakPtr(SharedPtr<T>& other);
    WeakPtr(WeakPtr<T>& other);
    WeakPtr(WeakPtr<T>&& other);
    WeakPtr<T>& operator=(WeakPtr<T>& other);
    WeakPtr<T>& operator=(WeakPtr<T>&& other);
    ~WeakPtr();

    void reset();
    int use_count();
    bool expired();
    SharedPtr<T> upgrade();
private:
    T* ptr_;
    int* ref_count;
};

// implementing weak_ptr
template <typename T>
WeakPtr<T>::WeakPtr() : ptr_(nullptr), ref_count(nullptr) {}

template <typename T>
WeakPtr<T>::WeakPtr(SharedPtr<T>& other) : ptr_(other.get()), ref_count(other.use_count_ptr()) {}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>& ptr) : ptr_(ptr.ptr_), ref_count(ptr.ref_count) {}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& ptr) : ptr_(ptr.ptr_), ref_count(ptr.ref_count) {
    // clear the other
    ptr.reset();
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>& other) {
    ptr_ = other.ptr_;
    ref_count = other.ref_count;
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) {
    ptr_ = other.ptr_;
    ref_count = other.ref_count;
    other.reset();
    return *this;
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
    reset();
}

template <typename T>
int WeakPtr<T>::use_count() {
    if (ref_count == nullptr) {
        return -1;
    }
    return *ref_count;
}

template <typename T>
bool WeakPtr<T>::expired() {
    return (ref_count == nullptr || *ref_count == 0);
}

template <typename T>
SharedPtr<T> WeakPtr<T>::upgrade() {
    // NOTE THIS IS UNDEFINED BEHAVIOR
    // REF_COUNT MAY BE A DANGLING POINTER
    if (expired()) {
        return SharedPtr<T> {};
    }
    ++*ref_count;
    return SharedPtr<T> {ptr_, ref_count};
}

template <typename T>
void WeakPtr<T>::reset() {
    ptr_ = nullptr;
    ref_count = nullptr;
}

#endif
