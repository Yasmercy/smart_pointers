#ifndef REFERENCE_MANAGER_H
#define REFERENCE_MANAGER_H

template <typename T>
class ReferenceManager {
public:
    ReferenceManager() = default;
    ReferenceManager(T* ptr) : ptr(ptr) {}
    ~ReferenceManager() {
        std::cout << "deleting ref manager: " << this << "\n";
    }

    void increment_shared() {
        ++shared_count_;
    }

    void decrement_shared() {
        --shared_count_;
        safe_reset();
    }

    void increment_weak() {
        ++weak_count_;
    }

    void decrement_weak() {
        --weak_count_;
        safe_reset();
    }

    int weak_count() {
        return weak_count_;
    }

    int shared_count() {
        return shared_count_;
    }

    T* get() {
        return ptr;
    }

    bool empty() {
        return ptr == nullptr && shared_count_ == 0 && weak_count_ == 0;
    }
    

private:
    void reset() {
        if (ptr != nullptr) {
            delete ptr;
        }
        shared_count_ = 0;
        weak_count_ = 0;
        ptr = nullptr;
    }
    
    bool expired() {
        return shared_count_ <= 0;
    }

    void safe_reset() {
        if (expired()) {
            reset();
        }
    }

    T* ptr = nullptr;
    int shared_count_ = 0;
    int weak_count_ = 0;
};

#endif
