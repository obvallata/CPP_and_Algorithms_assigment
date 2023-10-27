#include <algorithm>
#include <iostream>
#include <vector>

enum Colour { WHITE, GRAY, BLACK };

class Graph {
 public:
  explicit Graph(int am_ver) : edges_(am_ver), colours_(am_ver, WHITE){};

  void AddEdge(int ver_f, int ver_s) {
    edges_[ver_f].emplace_back(ver_s);
  }

  void PrintBrokenAmount() {
    for (size_t i = 0; i < edges_.size(); ++i) {
      if (colours_[i] == WHITE) {
        AllCyclesDFS(static_cast<int>(i));
      }
    }
    std::cout << cycle_counter_;
  }

 private:
  void AllCyclesDFS(int ver) {
    colours_[ver] = GRAY;
    for (auto& elem : edges_[ver]) {
      if (colours_[elem] == GRAY) {
        ++cycle_counter_;
      }
      if (colours_[elem] == WHITE) {
        AllCyclesDFS(elem);
      }
    }
    colours_[ver] = BLACK;
  }

  std::vector<std::vector<int>> edges_;
  std::vector<int> colours_;
  int cycle_counter_ = 0;
};

int main() {
  int am_ver = 0;
  int curr_ver = 0;
  std::cin >> am_ver;
  Graph graph(am_ver);
  for (int i = 0; i < am_ver; ++i) {
    std::cin >> curr_ver;
    graph.AddEdge(curr_ver - 1, i);
  }
  graph.PrintBrokenAmount();
  return 0;
}