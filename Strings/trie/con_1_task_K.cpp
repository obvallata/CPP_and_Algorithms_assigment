#include <iostream>
#include <map>
#include <vector>

struct Node {
  bool is_terminal = false;
  std::map<char, int> next;
};

class Trie {
 public:
  Trie() : trie_body_(1){}
  void Insert(const std::string& str) {
    int curr_node_ind = 0;
    for (auto& symb : str) {
      int next_node_ind = trie_body_[curr_node_ind].next[symb];
      if (next_node_ind == 0) {
        trie_body_.push_back(Node{});
        next_node_ind = trie_body_[curr_node_ind].next[symb] = static_cast<int>(trie_body_.size()) - 1;
      }
      curr_node_ind = next_node_ind;
    }
    trie_body_[curr_node_ind].is_terminal = true;
  }
  bool Find(const std::string& str) {
    int curr_node_ind = 0;
    for (auto& symb : str) {
      int next_node_ind = trie_body_[curr_node_ind].next[symb];
      if (next_node_ind == 0) {
        return false;
      }
      curr_node_ind = next_node_ind;
    }
    return trie_body_[curr_node_ind].is_terminal;
  }

  void Print() {
    for (size_t i = 0; i < trie_body_.size(); ++i) {
      if (trie_body_[i].is_terminal) {
        std::cout << 't';
      }
      std::cout << i << ' ';
      for (auto& way : trie_body_[i].next) {
        std::cout << "->" << way.first << "->" << way.second << ' ';
      }
    }
  }

 private:
  std::vector<Node> trie_body_;
};

int main() {
  std::iostream::sync_with_stdio(false);
  std::string input;
  std::cin >> input;
  Trie hash_without_hashing;
  while (input != "#") {
    std::string curr_word;
    std::cin >> curr_word;
    if (input == "+") {
      hash_without_hashing.Insert(curr_word);
    } else {
      auto is_in_trie = hash_without_hashing.Find(curr_word);
      if (is_in_trie) {
        std::cout << "YES\n";
      } else {
        std::cout << "NO\n";
      }
    }
    std::cin >> input;
  }
  return 0;
}
