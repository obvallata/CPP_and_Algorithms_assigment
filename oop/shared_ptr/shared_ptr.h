#ifndef OOP_ASSIGNMENTS_SHARED_PTR_H
#define OOP_ASSIGNMENTS_SHARED_PTR_H
#define WEAK_PTR_IMPLEMENTED
#include <stdexcept>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};
template <class T>
class SharedPtr;
template <class T>
class WeakPtr;
template <class T>
class Counter {
  size_t strong_ = 1;
  size_t weak_ = 0;
  Counter() = default;
  friend SharedPtr<T>;
  friend WeakPtr<T>;
};

template <class T>
class SharedPtr {
 public:
  SharedPtr() : data_(nullptr), counter_(nullptr) {
  }
  explicit SharedPtr(const WeakPtr<T>& other) {
    data_ = other.Get();
    counter_ = other.GetCounter();
    if (other.Expired()) {
      throw BadWeakPtr{};
    }
    if (counter_ != nullptr) {
      ++(counter_->strong_);
    }
  }
  explicit SharedPtr(T* data) : data_(data) {
    if (data_ != nullptr) {
      counter_ = new Counter<T>;
    }
  }
  SharedPtr(std::nullptr_t) : data_(nullptr), counter_(nullptr) {  // NOLINT
  }
  explicit SharedPtr(const T&& data) = delete;
  SharedPtr(const SharedPtr& other) {
    data_ = other.data_;
    counter_ = other.counter_;
    if (data_ != nullptr) {
      ++(counter_->strong_);
    }
  };
  SharedPtr(SharedPtr&& other) noexcept : SharedPtr() {
    Swap(other);
    other.data_ = nullptr;
    other.counter_ = nullptr;
  }
  bool operator==(const SharedPtr& other) {
    return data_ == other.data_ && counter_ == other.counter_;
  }
  SharedPtr& operator=(const SharedPtr& other) {
    if (*this == other) {
      return *this;
    }
    Destructor();
    data_ = other.data_;
    counter_ = other.counter_;
    if (data_ != nullptr) {
      ++(counter_->strong_);
    }
    return *this;
  }
  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (*this == other) {
      return *this;
    }
    Destructor();
    Swap(other);
    other.data_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }
  T& operator*() const {
    return *data_;
  }
  T* operator->() const {
    return data_;
  }
  void Reset(T* ptr = nullptr) {
    Destructor();
    data_ = ptr;
    counter_ = nullptr;
    if (data_ != nullptr) {
      counter_ = new Counter<T>;
    }
  }
  void Swap(SharedPtr<T>& other) {
    std::swap(data_, other.data_);
    std::swap(counter_, other.counter_);
  }
  T* Get() const {
    return data_;
  }
  explicit operator bool() const {
    return static_cast<bool>(data_);
  }
  size_t UseCount() const {
    if (data_ != nullptr) {
      return counter_->strong_;
    }
    return 0;
  }
  void Destructor() {
    if (data_ != nullptr) {
      --(counter_->strong_);
      if (counter_->strong_ == 0) {
        delete data_;
        if (counter_->weak_ == 0) {
          delete counter_;
        }
      }
    }
  }
  ~SharedPtr() {
    Destructor();
  }
  friend WeakPtr<T>;

 private:
  Counter<T>* GetCounter() const {
    return counter_;
  }
  T* data_ = nullptr;
  Counter<T>* counter_ = nullptr;
};

template <class T>
class WeakPtr {
 public:
  WeakPtr() : data_(nullptr), counter_(nullptr) {
  }
  explicit WeakPtr(T* data) : data_(data) {
    if (data_ != nullptr) {
      counter_ = new Counter<T>;
      --(counter_->strong_);
    }
  }
  explicit WeakPtr(std::nullptr_t) : data_(nullptr), counter_(nullptr) {
  }
  explicit WeakPtr(const T&& data) = delete;
  WeakPtr(const WeakPtr& other) {
    data_ = other.data_;
    counter_ = other.counter_;
    if (data_ != nullptr) {
      ++(counter_->weak_);
    }
  };
  WeakPtr(WeakPtr&& other) noexcept : WeakPtr() {
    Swap(other);
    other.data_ = nullptr;
    other.counter_ = nullptr;
  }
  WeakPtr(const SharedPtr<T>& other) {  // NOLINT
    data_ = other.Get();
    counter_ = other.GetCounter();
    if (counter_ != nullptr) {
      ++(counter_->weak_);
    }
  }
  bool operator==(const WeakPtr& other) {
    return data_ == other.data_ && counter_ == other.counter_;
  }
  WeakPtr& operator=(const WeakPtr& other) {
    if (*this == other) {
      return *this;
    }
    Destructor();
    data_ = other.data_;
    counter_ = other.counter_;
    if (data_ != nullptr) {
      ++(counter_->weak_);
    }
    return *this;
  }
  WeakPtr& operator=(WeakPtr&& other) noexcept {
    if (*this == other) {
      return *this;
    }
    Destructor();
    Swap(other);
    other.data_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }
  void Reset(T* ptr = nullptr) {
    Destructor();
    data_ = ptr;
    counter_ = nullptr;
    if (data_ != nullptr) {
      counter_ = new Counter<T>;
      --(counter_->strong_);
    }
  }
  void Swap(WeakPtr<T>& other) {
    std::swap(data_, other.data_);
    std::swap(counter_, other.counter_);
  }
  bool Expired() const {
    if (counter_ != nullptr) {
      return counter_->strong_ == 0;
    }
    return true;
  }
  size_t UseCount() const {
    if (data_ != nullptr) {
      return counter_->strong_;
    }
    return 0;
  }
  SharedPtr<T> Lock() const {
    return (Expired() ? SharedPtr<T>(nullptr) : SharedPtr<T>(*this));
  }
  ~WeakPtr() {
    Destructor();
  }
  void Destructor() {
    if (counter_ != nullptr) {
      --(counter_->weak_);
      if (counter_->weak_ == 0 && counter_->strong_ == 0) {
        delete counter_;
      }
    }
  }
  friend SharedPtr<T>;

 private:
  Counter<T>* GetCounter() const {
    return counter_;
  }
  T* Get() const {
    return data_;
  }
  T* data_ = nullptr;
  Counter<T>* counter_ = nullptr;
};

template <class T, class... Args>
SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
#endif
