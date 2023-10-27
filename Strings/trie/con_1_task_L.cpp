#include <iostream>
#include <map>
#include <queue>

struct Node {
  bool is_terminal = false;
  std::map<char, int> next;
  int suf_link = -1;
  int exit_link = -1;
};

struct StepInQueue {
  char symb;
  int parent_ind;
  int curr_node_ind;
};

class Trie {
 public:
  Trie() : trie_body_(1){}
  void Insert(const std::string& str) {
    int curr_node_ind = 0;
    for (auto& symb : str) {
      int next_node_ind = trie_body_[curr_node_ind].next[symb];
      if (next_node_ind == 0) {
        trie_body_.emplace_back(Node{});
        next_node_ind = trie_body_[curr_node_ind].next[symb] = static_cast<int>(trie_body_.size()) - 1;
      }
      curr_node_ind = next_node_ind;
    }
    trie_body_[curr_node_ind].is_terminal = true;
  }

  void BuildSuffixReferences() {
    std::queue<StepInQueue> queue;
    for (auto& elem : trie_body_[0].next) {
      queue.emplace(StepInQueue{elem.first, 0, elem.second});
    }
    while (!queue.empty()) {
      auto queue_elem = queue.front();
      queue.pop();
      int suf = trie_body_[queue_elem.parent_ind].suf_link;
      while (suf != -1 && trie_body_[suf].next.find(queue_elem.symb) == trie_body_[suf].next.end()) {
        suf = trie_body_[suf].suf_link;
      }
      if (suf == -1) {
        trie_body_[queue_elem.curr_node_ind].suf_link = 0;
      } else {
        if (trie_body_[suf].next.find(queue_elem.symb) == trie_body_[suf].next.end()) {
          trie_body_[queue_elem.curr_node_ind].suf_link = 0;
        } else {
          trie_body_[queue_elem.curr_node_ind].suf_link = trie_body_[suf].next[queue_elem.symb];
        }
      }
      for (auto& elem : trie_body_[queue_elem.curr_node_ind].next) {
        queue.emplace(StepInQueue{elem.first, queue_elem.curr_node_ind, elem.second});
      }
    }
  }

  void BuildExitLinks() {
    std::queue<int> queue;
    for (auto& elem : trie_body_[0].next) {
      queue.emplace(elem.second);
    }
    while (!queue.empty()) {
      int curr_node = queue.front();
      queue.pop();
      int suf = trie_body_[curr_node].suf_link;
      if (suf != 0) {
        if (trie_body_[suf].is_terminal) {
          trie_body_[curr_node].exit_link = suf;
        } else {
          trie_body_[curr_node].exit_link = trie_body_[suf].exit_link;
        }
      }
      for (auto& elem : trie_body_[curr_node].next) {
        queue.emplace(elem.second);
      }
    }
  }
  size_t Size() const {
    return trie_body_.size();
  }

  int Find(const std::string& str) {
    int curr_node_ind = 0;
    for (auto& symb : str) {
      int next_node_ind = trie_body_[curr_node_ind].next[symb];
      if (next_node_ind == 0) {
        return -1;
      }
      curr_node_ind = next_node_ind;
    }
    if (trie_body_[curr_node_ind].is_terminal) {
      return curr_node_ind;
    }
    return -1;
  }

  void Print() const {
    for (size_t i = 0; i < trie_body_.size(); ++i) {
      if (trie_body_[i].is_terminal) {
        std::cout << 't';
      }
      std::cout << i << ' ';
      for (auto& way : trie_body_[i].next) {
        std::cout << "->" << way.first << "->" << way.second << ' ';
      }
      std::cout << "suf_ref" << trie_body_[i].suf_link << ' ' << "exit_link" << trie_body_[i].exit_link << '\n';
    }
  }

  friend int NextState(Trie& trie, int node, char symb);
  friend void PrintWordsIndexes(std::vector<std::vector<int>>& w_coor, Trie& trie, int ind, int node);

 private:
  std::vector<Node> trie_body_;
};

int NextState(Trie& trie, int node, char symb) {
  while (node != -1 && trie.trie_body_[node].next.count(symb) == 0) {
    node = trie.trie_body_[node].suf_link;
  }
  if (node == -1) {
    return 0;
  }
  return trie.trie_body_[node].next[symb];
}

void PrintWordsIndexes(std::vector<std::vector<int>>& w_coor, Trie& trie, int ind, int node) {
  if (!trie.trie_body_[node].is_terminal) {
    node = trie.trie_body_[node].exit_link;
  }
  while (node != -1) {
    w_coor[node].emplace_back(ind);
    node = trie.trie_body_[node].exit_link;
  }
}

void AhoCorasik(const std::string& str, const std::vector<std::string>& words) {
  Trie dictionary;
  for (auto& elem : words) {
    dictionary.Insert(elem);
  }
  dictionary.BuildSuffixReferences();
  dictionary.BuildExitLinks();
  std::vector<std::vector<int>> words_coords(dictionary.Size());
  int node = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    node = NextState(dictionary, node, str[i]);
    PrintWordsIndexes(words_coords, dictionary, static_cast<int>(i), node);
  }
  for (auto& word : words) {
    int word_vertex = dictionary.Find(word);
    std::cout << words_coords[word_vertex].size() << ' ';
    for (auto& num :  words_coords[word_vertex]) {
      std::cout << num - static_cast<int>(word.size()) + 2 << ' ';
    }
    std::cout << '\n';
  }

}

int main() {
  std::iostream::sync_with_stdio(false);
  std::string input;
  std::cin >> input;
  size_t amount = 0;
  std::cin >> amount;
  std::string curr_word;
  std::vector<std::string> words(amount);
  for (size_t i = 0; i < amount; ++i) {
    std::cin >> words[i];
  }
  AhoCorasik(input, words);
  return 0;
}