#include <iostream>
#include "cppstring.h"

char* Concatenation(const char symbol, size_t size) {
  auto sequence = new char[size];
  for (size_t i = 0; i < size; ++i) {
    sequence[i] = symbol;
  }
  return sequence;
}
void String::Expand() {
  if (capacity_ != 0) {
    capacity_ *= 2;
    auto new_buffer = new char[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      new_buffer[i] = buffer_[i];
    }
    delete[] buffer_;
    buffer_ = new_buffer;
  } else {
    capacity_ = 1;
    auto new_buffer = new char[capacity_];
    buffer_ = new_buffer;
  }
}
size_t String::Length(const char* str) {
  size_t i = 0;
  while (str[i] != 0) {
    ++i;
  }
  return i;
}

String::String() {
  size_ = 0;
  capacity_ = 0;
  buffer_ = nullptr;
}
String::String(const char* content, const size_t size) {
  size_ = size;
  capacity_ = size;
  if (capacity_ == 0) {
    buffer_ = nullptr;
  } else {
    buffer_ = new char[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = content[i];
    }
  }
}
String::String(const size_t size, const char symbol) {
  size_ = size;
  capacity_ = size;
  if (capacity_ > 0) {
    buffer_ = new char[capacity_];
  } else {
    buffer_ = nullptr;
  }
  for (size_t i = 0; i < size_; ++i) {
    buffer_[i] = symbol;
  }
}
String::String(const char* content) : String(content, Length(content)) {
}

String::String(const String& old_str) {
  size_ = old_str.size_;
  capacity_ = old_str.capacity_;
  if (capacity_ == 0) {
    buffer_ = nullptr;
  } else {
    buffer_ = new char[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = old_str.buffer_[i];
    }
  }
}
String& String::operator=(const String& old_str) {
  if (old_str.buffer_ == this->buffer_) {
    return *this;
  }
  size_ = old_str.size_;
  capacity_ = old_str.capacity_;
  if (capacity_ == 0) {
    delete[] buffer_;
    buffer_ = nullptr;
  } else {
    delete[] buffer_;
    buffer_ = new char[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      buffer_[i] = old_str.buffer_[i];
    }
  }
  return *this;
}
String::~String() {
  delete[] buffer_;
}

char& String::operator[](const size_t index) {
  return buffer_[index];
}
char String::operator[](const size_t index) const {
  return buffer_[index];
}
char& String::At(const size_t index) {
  if (index >= size_) {
    throw StringOutOfRange();
  }
  return buffer_[index];
}
char String::At(const size_t index) const {
  if (index >= size_) {
    throw StringOutOfRange();
  }
  return buffer_[index];
}
char String::Front() const {
  return buffer_[0];
}
char& String::Front() {
  return buffer_[0];
}
char String::Back() const {
  return buffer_[size_ - 1];
}
char& String::Back() {
  return buffer_[size_ - 1];
}
const char* String::CStr() const {
  return buffer_;
}
char* String::CStr() {
  return buffer_;
}
char* String::Data() {
  return buffer_;
}
const char* String::Data() const {
  return buffer_;
}
bool String::Empty() const {
  return size_ == 0;
}
size_t String::Size() const {
  return size_;
}
size_t String::Length() const {
  return size_;
}
size_t String::Capacity() const {
  return capacity_;
}
void String::Clear() {
  size_ = 0;
}
void String::Swap(String& other) {
  auto tmp = other;
  other = *this;
  *this = tmp;
}
void String::PopBack() {
  if (size_ > 0) {
    --size_;
  }
}
void String::PushBack(const char symbol) {
  if (size_ == capacity_) {
    this->Expand();
  }
  buffer_[size_] = symbol;
  ++size_;
}
String& String::operator+=(const String& other) {
  size_t size = other.size_;
  for (size_t i = 0; i < size && other.capacity_ != 0; ++i) {
    this->PushBack(other.buffer_[i]);
  }
  return *this;
}
void String::Resize(const size_t new_size, const char symbol) {
  if (size_ == new_size) {
    return;
  }
  if (size_ > new_size) {
    size_ = new_size;
    return;
  }
  if (capacity_ == 0) {
    *this = String(new_size, symbol);
    return;
  }
  *this += String(new_size - size_, symbol);
}
void String::Reserve(const size_t new_capacity) {
  if (capacity_ >= new_capacity) {
    return;
  }
  auto new_buffer = new char[new_capacity];
  for (size_t i = 0; i < size_; ++i) {
    new_buffer[i] = buffer_[i];
  }
  delete[] buffer_;
  capacity_ = new_capacity;
  buffer_ = new_buffer;
}
void String::ShrinkToFit() {
  if (capacity_ > size_) {
    *this = String(this->buffer_, size_);
  }
}
String operator+(const String& first_str, const String& second_str) {
  auto result = String(first_str);
  result += second_str;
  return result;
}
bool operator<(const String& first_str, const String& second_str) {
  bool answer = false;
  bool eq = true;
  size_t i = 0;
  size_t s_one = first_str.Size();
  size_t s_two = second_str.Size();
  while (i < s_one && i < s_two) {
    if (first_str[i] != second_str[i]) {
      eq = false;
      break;
    }
    ++i;
  }
  if (!eq) {
    answer = first_str[i] < second_str[i];
  } else {
    answer = s_one < s_two;
  }
  return answer;
}
bool operator>(const String& first_str, const String& second_str) {
  return !(first_str == second_str) && !(first_str < second_str);
}
bool operator<=(const String& first_str, const String& second_str) {
  return (first_str < second_str) || (first_str == second_str);
}
bool operator>=(const String& first_str, const String& second_str) {
  return (first_str > second_str) || (first_str == second_str);
}
bool operator==(const String& first_str, const String& second_str) {
  auto buffer_one = first_str.Data();
  auto buffer_two = second_str.Data();
  auto size_one = first_str.Size();
  auto size_two = second_str.Size();
  if (size_two != size_one) {
    return false;
  }
  if (size_one == 0) {
    return true;
  }
  bool answer = true;
  for (size_t i = 0; i < size_one && answer; ++i) {
    if (buffer_one[i] != buffer_two[i]) {
      answer = false;
    }
  }
  return answer;
}
bool operator!=(const String& first_str, const String& second_str) {
  return !(first_str == second_str);
}
std::ostream& operator<<(std::ostream& os, const String& value) {
  size_t size = value.Size();
  auto data = value.Data();
  for (size_t i = 0; i < size; ++i) {
    os << data[i];
  }
  return os;
}