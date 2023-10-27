#pragma once
#include <cstdint>
#include <iostream>
#include <random>

class UniversalHash {
 public:
  explicit UniversalHash(const size_t a = 1, const size_t b = 0) : coefficient_a_(a), coefficient_b_(b){};

  template <class ValueType>
  size_t operator()(const ValueType& value) const {
    return ((coefficient_a_ * value + coefficient_b_) % kPrime) % kMaxValue;
  }

  template <class Generator>
  static UniversalHash GenerateHash(Generator& generator) {
    auto a = generator() % kMaxValue;
    auto b = generator() % kMaxValue;
    return UniversalHash(a, b);
  }

 private:
  size_t coefficient_a_ = 1;
  size_t coefficient_b_ = 0;
  static const size_t kPrime = 1073676287;

 public:
  static const size_t kMaxValue = kPrime - 1;
};