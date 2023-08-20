#ifndef MY_UNIQUE_PTR_H
#define MY_UNIQUE_PTR_H

template <typename T>
class UniquePtr {
public:
    // rule of 5
    UniquePtr();
    UniquePtr(T* p);
    // move/copy constructor
    UniquePtr(UniquePtr<T>& other);
    UniquePtr(UniquePtr<T>&& other);
    // assignment operators
    UniquePtr<T>& operator=(UniquePtr<T>& other);
    UniquePtr<T>& operator=(UniquePtr<T>&& other);
    // destructor
    ~UniquePtr();

    // helpful methods
    T& operator*();
    T* operator->();
    void reset(T* p);
    T* get();

private:
    T* ptr_;

};

// implementing UniquePtr
template <typename T>
UniquePtr<T>::UniquePtr() : ptr_(nullptr) {}

template <typename T>
UniquePtr<T>::UniquePtr(T* p) : ptr_(p) {}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T>& other) : ptr_(nullptr) {
    // this is the copy constructor
    // creates a deepcopy of the data stored in other
    if (this == &other) {
        return;
    }

    if (other == nullptr) {
        return;
    };
    ptr_ = new T {*ptr_};
}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& other) : ptr_(other.ptr_) {
    // this is the move constructor
    // takes ownership of the data in other
    // and resets data
    other.ptr_ = nullptr;
}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>& other) {
    // this is the copy assignment operator
    // deepcopes the data inside other
    // deletes the data already stored
    if (ptr_ != nullptr) {
        delete ptr_;
    }
    ptr_ = new T {*other};
    return *this;
}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& other) {
    // this is the move assignment operator
    if (ptr_ != nullptr) {
        delete ptr_;
    }
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    // deleting the data stored inside ptr
    delete ptr_;
}

template <typename T>
T& UniquePtr<T>::operator*() {
    if (ptr_ == nullptr) {
        throw std::runtime_error("dereferencing nullptr");
    }
    return *ptr_;
}

template <typename T>
T* UniquePtr<T>::operator->() {
    if (ptr_ == nullptr) {
        throw std::runtime_error("dereferencing nullptr");
    }
    return ptr_;
}

template <typename T>
void UniquePtr<T>::reset(T* p) {
    // deletes the data currently owned
    // and changes ownership to a new value
    delete ptr_;
    ptr_ = p;
}

template <typename T>
T* UniquePtr<T>::get() {
    return ptr_;
}

template <typename T>
bool operator==(UniquePtr<T>& p1, UniquePtr<T>& p2) {
    return p1.get() == p2.get();
}

template <typename T>
bool operator==(UniquePtr<T>& p1, T* p2) {
    return p1.get() == p2;
}

#endif
