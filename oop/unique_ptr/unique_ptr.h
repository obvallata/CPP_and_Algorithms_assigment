#ifndef OOP_ASSIGNMENTS_UNIQUE_PTR_H
#define OOP_ASSIGNMENTS_UNIQUE_PTR_H

template <class T>
class UniquePtr {
 public:
  explicit UniquePtr() : data_(nullptr) {
  }
  explicit UniquePtr(T* data) : data_(data) {
  }
  explicit UniquePtr(const T data) {
    T* ptr = &data;
    data_ = ptr;
  }
  explicit UniquePtr(const T&& data) = delete;
  UniquePtr(const UniquePtr& other) = delete;
  UniquePtr(UniquePtr&& other) noexcept {
    data_ = other.data_;
    other.data_ = nullptr;
  }
  UniquePtr& operator=(const UniquePtr& other) = delete;
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (other.data_ == data_) {
      return *this;
    }
    delete data_;
    data_ = other.data_;
    other.data_ = nullptr;
    return *this;
  }
  T& operator*() const {
    return *data_;
  }
  T* operator->() const {
    return data_;
  }
  T* Release() {
    T* answer = this->data_;
    data_ = nullptr;
    return answer;
  }
  void Reset(T* ptr = nullptr) {
    delete data_;
    data_ = ptr;
  }
  void Swap(UniquePtr<T>& other) {
    T* tmp = std::move(other.data_);
    other.data_ = std::move(data_);
    data_ = std::move(tmp);
  }
  T* Get() const {
    return data_;
  }
  explicit operator bool() const {
    return static_cast<bool>(data_);
  }
  ~UniquePtr() {
    delete data_;
  }

 private:
  T* data_ = nullptr;
};

#endif  // OOP_ASSIGNMENTS_UNIQUE_PTR_H
