#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H

#include "reference_manager.h"
#include <stdexcept>

template <typename T>
class SharedPtr {
public:
    // rule of 5
    SharedPtr() = default;
    SharedPtr(T* ptr);
    SharedPtr(ReferenceManager<T>* ref);
    SharedPtr(SharedPtr<T>& other);
    SharedPtr(SharedPtr<T>&& other);
    SharedPtr<T>& operator=(SharedPtr<T>& other);
    SharedPtr<T>& operator=(SharedPtr<T>&& other);
    ~SharedPtr();

    // methods
    T* get();
    T* operator->();
    T& operator*();
    int use_count();
    ReferenceManager<T>* get_ref_ptr();
    void reset();
    void reset(T* ptr);
    bool unique();

private:
    ReferenceManager<T>* ref = nullptr;
};

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : ref(new ReferenceManager {ptr}) {
    ref->increment_shared();
}

template <typename T>
SharedPtr<T>::SharedPtr(ReferenceManager<T>* ref) : ref(ref) {
    if (ref != nullptr) {
        ref->increment_shared();
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>& other) : ref(other.ref) {
    if (ref != nullptr) {
        ref->increment_shared();
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) : ref(other.ref) {
    other.ref = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>& other) {
    reset();
    ref = other.ref;
    if (ref != nullptr) {
        ref->increment_shared();
    }
    return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) {
    reset();
    std::swap(ref, other.ref);
    return *this;
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    std::cout << "deleting shared ptr: " << this << "\n";
    reset();
}

template <typename T>
T* SharedPtr<T>::get() {
    return (ref == nullptr) ? nullptr : ref->get();
}

template <typename T>
T* SharedPtr<T>::operator->() {
    if (ref == nullptr) {
        throw std::runtime_error("Dereferencing nullptr");
    }
    return ref->get();
}

template <typename T>
T& SharedPtr<T>::operator*() {
    if (ref == nullptr) {
        throw std::runtime_error("Dereferencing nullptr");
    }
    return *ref->get();
}
template <typename T>
int SharedPtr<T>::use_count() {
    return (ref == nullptr) ? 0 : ref->shared_count();
}

template <typename T>
ReferenceManager<T>* SharedPtr<T>::get_ref_ptr() {
    return ref;
}

template <typename T>
void SharedPtr<T>::reset() {
    if (ref != nullptr) {
        ref->decrement_shared();
        if (ref->empty()) {
            delete ref;
        }
    }
    ref = nullptr;
}

template <typename T>
void SharedPtr<T>::reset(T* ptr) {
    if (ptr == this->get()) {
        return;
    }

    reset();
    ref = new ReferenceManager {ptr};
    ref->increment_shared();
}

template <typename T>
bool SharedPtr<T>::unique() {
    return ref == nullptr || ref->shared_count() == 1;
}

// implementing non-member functions
template <typename T>
bool operator==(SharedPtr<T>& x, SharedPtr<T>& y) {

    return x.get() == y.get();
}

#endif
