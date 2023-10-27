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
  size_t amount_steps = 0;
  std::cin >> amount_steps;
  std::string start_state;
  std::string final_state;
  std::cin >> start_state >> final_state;
  std::string concatenation_zero = start_state + '#' + final_state + '0' + final_state;
  std::string concatenation_one = start_state + '#' + final_state + '1' + final_state;
  auto intersection_zero = PrefixFunction(concatenation_zero);
  auto intersection_one = PrefixFunction(concatenation_one);
  bool zero_is_poss = false;
  bool one_is_poss = false;
  for (size_t i = 0; i < concatenation_one.size(); ++i) {
    if (intersection_zero[i] == start_state.size()) {
      zero_is_poss = true;
    }
    if (intersection_one[i] == start_state.size()) {
      one_is_poss = true;
    }
  }
  if (one_is_poss && zero_is_poss) {
    std::cout << "Random";
  } else if (zero_is_poss) {
    std::cout << "No";
  } else {
    std::cout << "Yes";
  }
  return 0;
}
