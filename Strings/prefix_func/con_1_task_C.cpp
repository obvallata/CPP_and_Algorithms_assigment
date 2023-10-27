#include <iostream>
#include <string>
#include <vector>
std::vector<int> ZFunctionToPrefixFunc(const std::vector<int>& z_func) {
  std::vector<int> prefix_func(z_func.size(), 0);
  for (size_t i = 1; i < z_func.size(); ++i) {
    if (z_func[i] > 0) {
      for(int j = z_func[i] - 1;(j >= 0) && (prefix_func[i + j] == 0); --j) {
        prefix_func[i + j] = static_cast<int>(j) + 1;
      }
    }
  }
  return prefix_func;
}
std::vector<int> ZFunction(const std::string& input) {
  std::vector<int> suffix_list(input.size(), 0);
  suffix_list[0] = input.size();
  int left = 1;
  int right = 1;
  for (int i = 1; i < static_cast<int>(suffix_list.size()); ++i) {
    suffix_list[i] = std::max(0, std::min(right - i, suffix_list[i - left]));
    while (i + suffix_list[i] < static_cast<int>(input.size()) && input[suffix_list[i]] == input[i + suffix_list[i]]) {
      ++suffix_list[i];
    }
    if (suffix_list[i] > right) {
      right = suffix_list[i];
      left = i;
    }
  }
  return suffix_list;
}

int main() {
  std::string input;
  std::cin >> input;
  auto z_func = ZFunction(input);
  for (auto & i : z_func) {
    std::cout << i << ' ';
  }
  return 0;
}
