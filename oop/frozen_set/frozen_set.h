#pragma once
#include <optional>
#include "universal_hash.h"
#include "unordered_set"
#include <vector>

template <class KeyT, class Hash>
class FrozenSet {
 public:
  FrozenSet() = default;

  template <class IteratorT, class Generator>
  FrozenSet(IteratorT first, IteratorT last, Generator generator) {
    size_ = std::distance(first, last);
    buckets_hash_func_.resize(size_);
    std::vector<std::vector<std::optional<KeyT>>> tmp_buckets(size_);
    hash_table_ = std::vector<std::vector<std::optional<KeyT>>>(size_);
    UniversalHash hash_func_high_lvl;
    bool buckets_size_not_acceptable = true;
    int64_t sum_squared_lengths = 0;
    do {
      hash_func_high_lvl = UniversalHash::GenerateHash(generator);
      for (auto iter = first; iter != last; ++iter) {
        auto current_index = hash_func_high_lvl(*iter) % size_;
        sum_squared_lengths -= tmp_buckets[current_index].size() * tmp_buckets[current_index].size();
        tmp_buckets[current_index].push_back(std::make_optional(*iter));
        sum_squared_lengths += tmp_buckets[current_index].size() * tmp_buckets[current_index].size();
      }
      buckets_size_not_acceptable = static_cast<size_t>(sum_squared_lengths) >= 4 * size_;
    } while (buckets_size_not_acceptable);
    high_lvl_func_ = hash_func_high_lvl;
    for (size_t i = 0; i < size_; ++i) {
      auto current_bucket_size = tmp_buckets[i].size() * tmp_buckets[i].size();
      UniversalHash bucket_hash_func;
      bool no_collisions = true;
      do {
        std::vector<std::optional<KeyT>> current_bucket(current_bucket_size);
        no_collisions = true;
        bucket_hash_func = UniversalHash::GenerateHash(generator);
        buckets_hash_func_[i] = bucket_hash_func;
        for (auto& elem : tmp_buckets[i]) {
          auto current_index = bucket_hash_func(*elem) % current_bucket_size;
          if (current_bucket[current_index].has_value()) {
            no_collisions = false;
            break;
          }
          current_bucket[current_index] = elem;
          if (elem == tmp_buckets[i][tmp_buckets[i].size() - 1] && no_collisions) {
            hash_table_[i] = std::move(current_bucket);
          }
        }
      } while (!no_collisions);
    }
  }

  size_t Size() const {
    return size_;
  }

  bool Empty() const {
    return size_ == 0;
  }

  void Clear() {
    hash_table_.clear();
    size_ = 0;
  }

  bool Find(const KeyT& key) const {
    if (size_ == 0) {
      return false;
    }
    auto bucket_index = high_lvl_func_(key) % size_;
    if (hash_table_[bucket_index].empty()) {
      return false;
    }
    return *hash_table_[bucket_index][buckets_hash_func_[bucket_index](key) % hash_table_[bucket_index].size()] == key;
  }

 private:
  std::vector<std::vector<std::optional<KeyT>>> hash_table_ = std::vector<std::vector<std::optional<KeyT>>>();
  UniversalHash high_lvl_func_ = UniversalHash();
  std::vector<UniversalHash> buckets_hash_func_ = std::vector<UniversalHash>();
  size_t size_ = 0;
};