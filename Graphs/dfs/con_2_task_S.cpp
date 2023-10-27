#include <iostream>
#include <vector>

enum Colour { WHITE, GRAY, BLACK };

class Graph {
 public:
  explicit Graph(int am_ver) : edges_(am_ver), colours_(am_ver, WHITE), amount_visited_(am_ver, 0){};

  void AddEdge(int ver_f, int ver_s) {
    edges_[ver_f].emplace_back(ver_s);
  }

  void PrintEulerCycle() {
    for (int i = 0; i < static_cast<int>(edges_.size()); ++i) {
      if (!edges_[i].empty()) {
        EulerCycle(i);
        break;
      }
    }
    std::cout << cycle_.size() << ' ';
    for (auto& ver : cycle_) {
      std::cout << ver + 1 << ' ';
    }
  }

  bool SeveralComponents() {
    for (int i = 0; i < static_cast<int>(edges_.size()); ++i) {
      if (!edges_[i].empty()) {
        DFS(i);
        break;
      }
    }
    for (int i = 0; i < static_cast<int>(edges_.size()); ++i) {
      if (!edges_[i].empty() && colours_[i] == WHITE) {
        return true;
      }
    }
    return false;
  }

 private:
  void EulerCycle(int ver) {
    while (amount_visited_[ver] != static_cast<int>(edges_[ver].size())) {
      EulerCycle(edges_[ver][amount_visited_[ver]++]);
    }
    cycle_.push_back(ver);
  }

  void DFS(int ver) {
    colours_[ver] = GRAY;
    for (auto& elem : edges_[ver]) {
      if (colours_[elem] == WHITE) {
        DFS(elem);
      }
    }
    colours_[ver] = BLACK;
  }

  std::vector<std::vector<int>> edges_;
  std::vector<int> colours_;
  std::vector<int> amount_visited_;
  std::vector<int> cycle_;
};

int main() {
  int am_bus_ways = 0;
  int am_squares = 0;
  int curr_amount = 0;
  int curr_stop = 0;
  int prev_stop = 0;
  std::cin >> am_bus_ways >> am_squares;
  Graph graph(am_squares);
  for (int i = 0; i < am_bus_ways; ++i) {
    std::cin >> curr_amount >> prev_stop;
    for (int j = 0; j < curr_amount; ++j) {
      std::cin >> curr_stop;
      graph.AddEdge(prev_stop - 1, curr_stop - 1);
      prev_stop = curr_stop;
    }
  }
  if (graph.SeveralComponents()) {
    std::cout << 0 << '\n';
  } else {
    graph.PrintEulerCycle();
  }
  return 0;
}