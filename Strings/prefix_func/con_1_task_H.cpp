#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> PrefixFunction(const std::string& input) {
  std::vector<size_t> prefix_list(input.size(), 0);
  for (size_t i = 1; i < prefix_list.size(); ++i) {
    size_t len_count = prefix_list[i - 1];
    while (len_count > 0 && input[i] != input[len_count]) {
      len_count = prefix_list[len_count - 1];
    }
    if (input[i] == input[len_count]) {
      ++len_count;
    }
    prefix_list[i] = len_count;
  }
  return prefix_list;
}

int main() {
  std::ios::sync_with_stdio(false);
  size_t amount = 0;
  std::cin >> amount;
  std::string prev_word;
  std::cin >> prev_word;
  for (size_t i = 1; i < amount; ++i) {
    std::string curr_word;
    std::cin >> curr_word;
    std::string to_compare;
    if (curr_word.size() >= prev_word.size()) {
      to_compare = prev_word;
    } else {
      to_compare.reserve(curr_word.size());
      for (size_t j = prev_word.size() - curr_word.size(); j < prev_word.size(); ++j) {
        to_compare += prev_word[j];
      }
    }
    auto intersection_suf = PrefixFunction(curr_word + '#' + to_compare);
    for (size_t j = intersection_suf.back(); j < curr_word.size(); ++j) {
      prev_word += curr_word[j];
    }
  }
  std::cout << prev_word;
  return 0;
}
