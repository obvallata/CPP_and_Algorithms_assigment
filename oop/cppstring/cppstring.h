#ifndef SEMINARI_2_SEMESTR_STR_H
#define SEMINARI_2_SEMESTR_STR_H
#include <iostream>
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 public:
  String();
  String(const char* content, size_t size);
  String(size_t size, const char symbol);
  String(const char* content);  // NOLINT
  String(const String& old_str);
  String& operator=(const String& old_str);
  ~String();
  char operator[](size_t index) const;
  char& operator[](size_t index);
  char At(size_t index) const;
  char& At(size_t index);
  char Front() const;
  char& Front();
  char Back() const;
  char& Back();
  char* CStr();
  char* Data();
  const char* CStr() const;
  const char* Data() const;
  bool Empty() const;
  size_t Size() const;
  size_t Length() const;
  size_t Capacity() const;
  void Clear();
  void Swap(String& other);
  void PopBack();
  void PushBack(const char symbol);

  void Expand();

  String& operator+=(const String& other);
  void Resize(size_t new_size, const char symbol);
  void Reserve(size_t new_capacity);
  void ShrinkToFit();
  size_t Length(const char* str);

 private:
  size_t size_;
  size_t capacity_;
  char* buffer_;
};

String operator+(const String& first_str, const String& second_str);
bool operator<(const String& first_str, const String& second_str);
bool operator>(const String& first_str, const String& second_str);
bool operator<=(const String& first_str, const String& second_str);
bool operator>=(const String& first_str, const String& second_str);
bool operator==(const String& first_str, const String& second_str);
bool operator!=(const String& first_str, const String& second_str);
std::ostream& operator<<(std::ostream& os, const String& value);

#endif
