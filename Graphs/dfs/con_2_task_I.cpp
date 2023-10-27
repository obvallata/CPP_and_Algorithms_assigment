#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
enum Colour { WHITE, GRAY, BLACK };

class Graph {
 public:
  explicit Graph(int am_ver) : edges_(am_ver), colours_(am_ver, WHITE), parent_(am_ver, -1){};

  void AddEdge(int ver_f, int ver_s) {
    edges_[ver_f].emplace_back(ver_s);
  }

  void PrintCycle() {
    std::vector<int> cycle;
    for (int ver = cycle_fi_; ver != cycle_st_; ver = parent_[ver]) {
      cycle.emplace_back(ver);
    }
    cycle.emplace_back(cycle_st_);
    reverse(cycle.begin(), cycle.end());
    for (int i : cycle) {
      std::cout << (i + 1) << ' ';
    }
  }

  bool HasCycle() {
    for (size_t i = 0; i < edges_.size(); ++i) {
      if (colours_[i] == WHITE) {
        if (HasCycleDFS(static_cast<int>(i))) {
          return true;
        }
      }
    }
    return false;
  }

 private:
  bool HasCycleDFS(int ver) {
    colours_[ver] = GRAY;
    for (auto& elem : edges_[ver]) {
      parent_[elem] = ver;
      if (colours_[elem] == GRAY) {
        cycle_st_ = elem;
        cycle_fi_ = ver;
        return true;
      }
      if (colours_[elem] == WHITE) {
        if (HasCycleDFS(elem)) {
          return true;
        }
      }
    }
    colours_[ver] = BLACK;
    return false;
  }

  std::vector<std::vector<int>> edges_;
  std::vector<int> colours_;
  std::vector<int> parent_;
  int cycle_st_ = -1;
  int cycle_fi_ = -1;
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
  if (graph.HasCycle()) {
    std::cout << "YES\n";
    graph.PrintCycle();
  } else {
    std::cout << "NO\n";
  }
  return 0;
}