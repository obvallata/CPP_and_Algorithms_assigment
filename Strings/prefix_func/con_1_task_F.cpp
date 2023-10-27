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

size_t AmountUniqueSubstr(const std::string& input) {
  size_t amount_uni_substr = 1;
  std::string substr;
  substr.reserve(input.size());
  substr += input[0];
  for (size_t i = 1; i < input.size(); ++i) {
    substr += input[i];
    std::string reverse_substr;
    reverse_substr.reserve(substr.size());
    for (auto j = substr.rbegin(); j != substr.rend(); ++j) {
      reverse_substr += *j;
    }
    auto reverse_pref_func = PrefixFunction(reverse_substr);
    size_t max_substr = reverse_pref_func[0];
    for (auto& j : reverse_pref_func) {
      if (max_substr < j) {
        max_substr = j;
      }
    }
    amount_uni_substr += substr.size() - max_substr;
  }
  return amount_uni_substr;
}

int main() {
  std::string input;
  std::cin >> input;
  std::cout << AmountUniqueSubstr(input);
  return 0;
}
