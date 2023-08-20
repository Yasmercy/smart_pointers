#ifndef MY_WEAK_PTR_H
#define MY_WEAK_PTR_H

#include "my_shared_ptr.h"
#include "reference_manager.h"

template <typename T>
class WeakPtr {
public:
    // rule of 5
    WeakPtr() = default;
    WeakPtr(SharedPtr<T>& other);
    WeakPtr(WeakPtr<T>& other);
    WeakPtr(WeakPtr<T>&& other);
    WeakPtr<T>& operator=(WeakPtr<T>& other);
    WeakPtr<T>& operator=(WeakPtr<T>&& other);
    ~WeakPtr();

    // methods
    int shared_count();
    bool expired();
    SharedPtr<T> upgrade();
    void reset();
private:
    ReferenceManager<T>* ref = nullptr;
};

template <typename T>
WeakPtr<T>::WeakPtr(SharedPtr<T>& other) : ref(other.get_ref_ptr()) {
    if (ref != nullptr) {
        ref->increment_weak();
    }
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>& other) : ref(other.ref) {
    if (ref != nullptr) {
        ref->increment_weak();
    }
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) : ref(other.ref) {
    if (ref != nullptr) {
        ref->increment_weak();
    }
    other.reset();
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>& other) {
    reset();
    ref = other.ref;
    if (ref != nullptr) {
        ref->increment_weak();
    }
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) {
    reset();
    ref = other.ref;
    if (ref != nullptr) {
        ref ->increment_weak();
    }
    other.reset();
    return *this;
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
    std::cout << "deleting weak ptr: " << this << "\n";
    reset();
}

template <typename T>
int WeakPtr<T>::shared_count() {
    return (ref == nullptr) ? 0 : ref->shared_count();
}

template <typename T>
bool WeakPtr<T>::expired() {
    return shared_count() == 0;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::upgrade() {
    return (expired()) ? SharedPtr<T> {} : SharedPtr<T> {ref};
}

template <typename T>
void WeakPtr<T>::reset() {
    if (ref == nullptr) {
        return;
    }

    ref->decrement_weak();
    if (ref->empty()) {
        delete ref;
    }
    ref = nullptr;
}

#endif
