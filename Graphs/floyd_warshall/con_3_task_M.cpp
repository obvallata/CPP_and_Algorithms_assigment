#include <iostream>
#include <map>
#include <queue>
#include <vector>

class DirectedGraph {
 public:
  explicit DirectedGraph(size_t am_ver) : edges_(am_ver) {
    for (size_t i = 0; i < am_ver; ++i) {
      edges_[i].resize(am_ver, k_max_dist_);
    }
  };

  void AddEdge(size_t, size_t, int);
  void FindAllMinDist();

  const std::vector<std::vector<int>>& GetMinDists() const {
    return edges_;
  }

 private:
  void FloydWarshall();
  const int k_max_dist_ = INT32_MAX;
  std::vector<std::vector<int>> edges_;
};

int main() {
  size_t amount_ver = 0;
  std::cin >> amount_ver;
  DirectedGraph graph(amount_ver);
  for (size_t i = 0; i < amount_ver; ++i) {
    for (size_t j = 0; j < amount_ver; ++j) {
      int curr_weight = 0;
      std::cin >> curr_weight;
      graph.AddEdge(i, j, curr_weight);
    }
  }
  graph.FindAllMinDist();
  for (size_t i = 0; i < amount_ver; ++i) {
    for (size_t j = 0; j < amount_ver; ++j) {
      std::cout << graph.GetMinDists()[i][j] << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}

void DirectedGraph::AddEdge(size_t ver_f, size_t ver_s, int weight) {
  edges_[ver_f][ver_s] = weight;
}

void DirectedGraph::FindAllMinDist() {
  FloydWarshall();
}

void DirectedGraph::FloydWarshall() {
  for (size_t i = 0; i < edges_.size(); ++i) {
    for (size_t j = 0; j < edges_.size(); ++j) {
      for (size_t k = 0; k < edges_.size(); ++k) {
        edges_[j][k] = std::min(edges_[j][k], edges_[j][i] + edges_[i][k]);
      }
    }
  }
}