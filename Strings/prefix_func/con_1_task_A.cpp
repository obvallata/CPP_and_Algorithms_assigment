#include <iostream>
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
  std::string input;
  std::cin >> input;
  auto prefix_func = PrefixFunction(input);
  for (auto & i : prefix_func) {
    std::cout << i << ' ';
  }
  return 0;
}

