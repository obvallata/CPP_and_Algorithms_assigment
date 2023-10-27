#pragma once
#include <forward_list>
#include <vector>
#define FORWARD_LIST_IMPLEMENTED
#define ITERATOR_IMPLEMENTED
template <class KeyT>
class UnorderedSet {
 public:
  using DifferenceType = std::ptrdiff_t;
  using Iterator = typename std::forward_list<KeyT>::const_iterator;
  using ConstIterator = typename std::forward_list<KeyT>::const_iterator;

  UnorderedSet() = default;

  explicit UnorderedSet(size_t size)
      : hash_table_(std::vector<Iterator>(size))
      , buckets_chain_(std::forward_list<KeyT>())
      , amount_elems_(0)
      , amount_buckets_(size){};

  void Insert(KeyT&& elem) {
    if (amount_buckets_ == 0) {
      Rehash(1);
    } else if (amount_elems_ == amount_buckets_) {
      Rehash(amount_buckets_ * 2);
    }
    InsertToRequiredTable(std::move(elem), *this);
  }

  void Insert(const KeyT& elem) {
    if (amount_buckets_ == 0) {
      Rehash(1);
    } else if (amount_elems_ == amount_buckets_) {
      Rehash(amount_buckets_ * 2);
    }
    InsertToRequiredTable(elem, *this);
  }

  void Erase(const KeyT& elem) {
    if (!Find(elem)) {
      return;
    }
    auto current_bucket = hash_table_[Bucket(elem)];
    auto prev_length = BucketSize(Bucket(elem));
    auto prev_iter = current_bucket;
    ++current_bucket;
    while (current_bucket != buckets_chain_.end()) {
      if (*current_bucket == elem) {
        buckets_chain_.erase_after(prev_iter);
        if (prev_length == 1) {
          hash_table_[std::hash<KeyT>{}(elem) % amount_buckets_] = Iterator();
          auto next = prev_iter;
          ++next;
          if (next != buckets_chain_.end()) {
            hash_table_[std::hash<KeyT>{}(*next) % amount_buckets_] = prev_iter;
          }
          break;
        }
        auto next = prev_iter;
        ++next;
        if (next != buckets_chain_.end() &&
            std::hash<KeyT>{}(*next) % amount_buckets_ != std::hash<KeyT>{}(elem) % amount_buckets_) {
          hash_table_[std::hash<KeyT>{}(*next) % amount_buckets_] = prev_iter;
        }
        break;
      }
      prev_iter = current_bucket;
      ++current_bucket;
    }
    --amount_elems_;
  }

  template <class IteratorT>
  UnorderedSet(IteratorT first, IteratorT last) {
    amount_buckets_ = std::distance(first, last);
    hash_table_.resize(amount_buckets_);
    for (auto& index = first; index != last; ++index) {
      Insert(*index);
    }
  }

  UnorderedSet(const UnorderedSet& other) {
    *this = other;
  }

  UnorderedSet(UnorderedSet&& other) noexcept {
    *this = std::move(other);
  }

  UnorderedSet& operator=(UnorderedSet&& other) noexcept {
    amount_buckets_ = other.amount_buckets_;
    amount_elems_ = other.amount_elems_;
    other.amount_elems_ = other.amount_buckets_ = 0;
    buckets_chain_ = std::move(other.buckets_chain_);
    hash_table_ = std::move(other.hash_table_);
    for (auto elem : buckets_chain_) {
      if (hash_table_[std::hash<KeyT>{}(elem) % amount_buckets_] == other.buckets_chain_.before_begin()) {
        hash_table_[std::hash<KeyT>{}(elem) % amount_buckets_] = buckets_chain_.before_begin();
        break;
      }
    }
    return *this;
  }

  UnorderedSet& operator=(const UnorderedSet& other) {
    amount_buckets_ = other.amount_buckets_;
    amount_elems_ = other.amount_elems_;
    hash_table_ = other.hash_table_;
    buckets_chain_ = other.buckets_chain_;
    return *this;
  }

  void Rehash(const size_t new_bucket_count) {
    if (new_bucket_count == amount_buckets_ || new_bucket_count < amount_elems_) {
      return;
    }
    amount_buckets_ = new_bucket_count;
    auto tmp_hash_table = UnorderedSet<KeyT>(amount_buckets_);
    for (auto index = buckets_chain_.begin(); index != buckets_chain_.end(); ++index) {
      InsertToRequiredTable(*index, tmp_hash_table);
    }
    *this = std::move(tmp_hash_table);
  }

  void Reserve(const size_t new_bucket_count) {
    if (amount_buckets_ < new_bucket_count) {
      Rehash(new_bucket_count);
    }
  }

  bool Find(const KeyT& elem) const {
    if (amount_buckets_ == 0 || amount_elems_ == 0) {
      return false;
    }
    auto poss_bucket = hash_table_[std::hash<KeyT>{}(elem) % amount_buckets_];
    if (BucketIsEmpty(poss_bucket)) {
      return false;
    }
    bool elem_exists = false;
    ++poss_bucket;
    while (poss_bucket != buckets_chain_.end()) {
      if (elem == *poss_bucket) {
        return true;
      }
      if (std::hash<KeyT>{}(elem) % amount_buckets_ != std::hash<KeyT>{}(*poss_bucket) % amount_buckets_) {
        return false;
      }
      ++poss_bucket;
    }
    return elem_exists;
  }

  size_t Size() const {
    return amount_elems_;
  }

  bool Empty() const {
    return amount_elems_ == 0;
  }

  void Clear() {
    hash_table_.clear();
    buckets_chain_.clear();
    amount_elems_ = amount_buckets_ = 0;
  }

  size_t BucketCount() const {
    return amount_buckets_;
  }

  size_t BucketSize(const size_t id) const {
    if (id >= amount_buckets_ || BucketIsEmpty(hash_table_[id])) {
      return 0;
    }
    size_t size = 0;
    auto first_elem_with_curr_hash = hash_table_[id];
    ++first_elem_with_curr_hash;
    while (first_elem_with_curr_hash != buckets_chain_.end()) {
      if (id != std::hash<KeyT>{}(*first_elem_with_curr_hash) % amount_buckets_) {
        break;
      }
      ++size;
      ++first_elem_with_curr_hash;
    }
    return size;
  }

  size_t Bucket(const KeyT& key) const {
    return amount_buckets_ == 0 ? 0 : std::hash<KeyT>{}(key) % amount_buckets_;
  }

  double LoadFactor() const {
    if (amount_buckets_ == 0) {
      return 0;
    }
    return static_cast<double>(amount_elems_) / static_cast<double>(amount_buckets_);
  }

  ~UnorderedSet() = default;

  Iterator begin() {  // NOLINT
    return buckets_chain_.begin();
  }

  ConstIterator begin() const {  // NOLINT
    return buckets_chain_.begin();
  }

  Iterator end() {  // NOLINT
    return buckets_chain_.end();
  }

  ConstIterator end() const {  // NOLINT
    return buckets_chain_.end();
  }

  ConstIterator cbegin() const {  // NOLINT
    return buckets_chain_.begin();
  }

  ConstIterator cend() const {  // NOLINT
    return buckets_chain_.end();
  }

 private:
  std::vector<Iterator> hash_table_ = std::vector<Iterator>();
  std::forward_list<KeyT> buckets_chain_ = std::forward_list<KeyT>();
  size_t amount_elems_ = 0;
  size_t amount_buckets_ = 0;

  bool BucketIsEmpty(const Iterator& bucket) const {
    return bucket == Iterator();
  }

  void InsertToRequiredTable(const KeyT& elem, UnorderedSet<KeyT>& container) {
    if (container.Find(elem)) {
      return;
    }
    ++container.amount_elems_;
    auto current_bucket = std::hash<KeyT>{}(elem) % container.amount_buckets_;
    if (BucketIsEmpty(container.hash_table_[current_bucket])) {
      if (container.buckets_chain_.begin() == container.buckets_chain_.end()) {
        container.buckets_chain_.push_front(elem);
        container.hash_table_[current_bucket] = container.buckets_chain_.before_begin();
      } else {
        auto very_first_before_insert = *container.buckets_chain_.begin();
        container.buckets_chain_.push_front(elem);
        container.hash_table_[current_bucket] = container.buckets_chain_.before_begin();
        container.hash_table_[std::hash<KeyT>{}(very_first_before_insert) % container.amount_buckets_] =
            container.buckets_chain_.begin();
      }
    } else {
      auto curr_hash_in_chain = container.hash_table_[current_bucket];
      buckets_chain_.insert_after(curr_hash_in_chain, elem);
    }
  }

  void InsertToRequiredTable(KeyT&& elem, UnorderedSet<KeyT>& container) {
    if (container.Find(elem)) {
      return;
    }
    ++container.amount_elems_;
    auto current_bucket = std::hash<KeyT>{}(elem) % container.amount_buckets_;
    if (BucketIsEmpty(container.hash_table_[current_bucket])) {
      if (container.buckets_chain_.begin() == container.buckets_chain_.end()) {
        container.buckets_chain_.push_front(std::move(elem));
        container.hash_table_[current_bucket] = container.buckets_chain_.before_begin();
      } else {
        auto very_first_before_insert = *container.buckets_chain_.begin();
        container.buckets_chain_.push_front(std::move(elem));
        container.hash_table_[current_bucket] = container.buckets_chain_.before_begin();
        container.hash_table_[std::hash<KeyT>{}(very_first_before_insert) % container.amount_buckets_] =
            container.buckets_chain_.begin();
      }
    } else {
      auto curr_hash_in_chain = container.hash_table_[current_bucket];
      buckets_chain_.insert_after(curr_hash_in_chain, std::move(elem));
    }
  }
};