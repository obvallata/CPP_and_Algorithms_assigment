#include <algorithm>
#include <iostream>
#include <vector>

enum Colour { WHITE, GRAY, BLACK };

class Graph {
 public:
  explicit Graph(int am_ver)
      : edges_(am_ver), reversed_edges_(am_ver), used_(am_ver, false), component_num_(am_ver, 0){};

  void AddEdge(int ver_f, int ver_s) {
    edges_[ver_f].emplace_back(ver_s);
    reversed_edges_[ver_s].emplace_back(ver_f);
  }

  void PrintComponents() {
    KosarajuAlg();
    std::cout << component_count_ << '\n';
    for (auto& elem : component_num_) {
      std::cout << elem << ' ';
    }
  }

 private:
  void KosarajuAlg() {
    used_ = std::vector<bool>(edges_.size(), false);
    for (int i = 0; i < static_cast<int>(edges_.size()); ++i) {
      if (!used_[i]) {
        FindOrder(i);
      }
    }
    used_ = std::vector<bool>(edges_.size(), false);
    std::reverse(ordered_.begin(), ordered_.end());
    for (auto& elem : ordered_) {
      if (!used_[elem]) {
        ++component_count_;
        FindComponent(elem);
      }
    }
  }
  void FindOrder(int ver) {
    used_[ver] = true;
    for (auto& elem : edges_[ver]) {
      if (!used_[elem]) {
        FindOrder(elem);
      }
    }
    ordered_.emplace_back(ver);
  }
  void FindComponent(int ver) {
    used_[ver] = true;
    component_num_[ver] = component_count_;
    for (auto& elem : reversed_edges_[ver]) {
      if (!used_[elem]) {
        FindComponent(elem);
      }
    }
  }

  std::vector<std::vector<int>> edges_;
  std::vector<std::vector<int>> reversed_edges_;
  std::vector<bool> used_;
  std::vector<int> ordered_;
  std::vector<int> component_num_;
  int component_count_ = 0;
};

int main() {
  int am_ver = 0;
  int am_edg = 0;
  int curr_st = 0;
  int curr_fi = 0;
  std::cin >> am_ver >> am_edg;
  Graph graph(am_ver);
  for (int i = 0; i < am_edg; ++i) {
    std::cin >> curr_st >> curr_fi;
    graph.AddEdge(curr_st - 1, curr_fi - 1);
  }
  graph.PrintComponents();
  return 0;
}