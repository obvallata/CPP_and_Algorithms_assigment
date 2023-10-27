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

std::string StringFromPrefixFunc(const std::vector<int>& prefix_func) {
  std::string recovered;
  recovered.reserve(prefix_func.size());
  if (!prefix_func.empty()) {
    recovered += 'a';
  }
  for (size_t i = 1; i < prefix_func.size(); ++i) {
    if (prefix_func[i] > 0) {
      recovered += recovered[prefix_func[i] - 1];
    } else {
      auto prev_symb = i - 1;
      std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
      std::vector<bool> used_letters(alphabet.size(), false);
      used_letters[0] = true;
      while (prefix_func[prev_symb] > 0) {
        prev_symb = prefix_func[prev_symb] - 1;
        used_letters[recovered[prev_symb + 1] - alphabet[0]] = true;
      }
      for (size_t j = 1; j < alphabet.size(); ++j) {
        if (!used_letters[j]) {
          recovered += alphabet[j];
          break;
        }
      }
    }
  }
  return recovered;
}

int main() {
  size_t amount = 0;
  std::cin >> amount;
  std::vector<int> z_func(amount, 0);
  for (auto& i : z_func) {
    std::cin >> i;
  }
  std::cout << StringFromPrefixFunc(ZFunctionToPrefixFunc(z_func));
  return 0;
}
