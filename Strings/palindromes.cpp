#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int64_t> EvenPalindromes(const std::vector<size_t>& input) {
  std::vector<int64_t> amount_pal(input.size(), 0);
  auto size = static_cast<int64_t>(input.size());
  int64_t left = 0;
  int64_t right = -1;
  for (int64_t i = 0; i < size - 1; ++i) {
    if (i < right) {
      amount_pal[i] = std::min(amount_pal[left + right - i - 1], right - i);
    }
    while ( i + amount_pal[i] + 1 < size && i - amount_pal[i] >= 0) {
      if (input[i - amount_pal[i]] == input[i + amount_pal[i] + 1]) {
        ++amount_pal[i];
      } else {
        break;
      }
    }
    if (i + amount_pal[i] > right) {
      left = i - amount_pal[i] + 1;
      right = i + amount_pal[i];
    }
  }
  return amount_pal;
}

int main() {
  std::ios::sync_with_stdio(false);
  size_t amount_cubes = 0;
  size_t amount_colors = 0;
  std::cin >> amount_cubes >> amount_colors;
  std::vector<size_t> cubes(amount_cubes);
  for (auto& i : cubes) {
    std::cin >> i;
  }
  auto palindromes = EvenPalindromes(cubes);
  for (auto& i : palindromes) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
  for (int i = static_cast<int>(palindromes.size()) / 2; i > 0; --i) {
    if (i - palindromes[i - 1] <= 0) {
      std::cout << palindromes.size() - i << ' ';
    }
  }
  std::cout << cubes.size();
  return 0;
}
