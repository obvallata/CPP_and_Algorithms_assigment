#include <vector>
#include <iostream>

class DirectedGraph {
 public:
  explicit DirectedGraph(size_t amount_ver) : amount_vertexes_(amount_ver) {
    edges_.reserve(amount_vertexes_);
  }
  void AddEdge(size_t from, size_t to, int64_t weight) {
    edges_.emplace_back(from, to, weight);
  }

  std::vector<int64_t> MinDistances() {
    return FordBellman();
  }

 private:
  struct Edge {
    size_t from_ = 0;
    size_t to_ = 0;
    int64_t weight_ = 0;
    Edge(size_t from, size_t to, int64_t weight) : from_(from), to_(to), weight_(weight){};
    Edge() = default;
  };

  bool RelaxEdge(const Edge& edge, std::vector<int64_t>& dist) const {
    if (dist[edge.from_] == k_max_weight_) {
      return false;
    }
    if (dist[edge.to_] > dist[edge.from_] + edge.weight_) {
      dist[edge.to_] = dist[edge.from_] + edge.weight_;
      return true;
    }
    return false;
  }

  std::vector<int64_t> FordBellman(size_t begin = 0) {
    std::vector<int64_t> dist(amount_vertexes_, k_max_weight_);
    dist[begin] = 0;
    for (size_t i = 0; i < amount_vertexes_ - 1; ++i) {
      for (auto& edge : edges_) {
        RelaxEdge(edge, dist);
      }
    }
    for (auto& elem : dist) {
      if (elem == k_max_weight_) {
        elem = no_way_exists_;
      }
    }
    return dist;
  }

  size_t amount_vertexes_ = 0;
  std::vector<Edge> edges_;
  const int64_t k_max_weight_ = INT64_MAX;
  const int64_t no_way_exists_ = 30000;
};

int main() {
  size_t amount_vertexes = 0;
  size_t amount_edges = 0;
  std::cin >> amount_vertexes >> amount_edges;
  DirectedGraph graph(amount_vertexes);
  for (size_t i = 0; i < amount_edges; ++i) {
    size_t curr_from = 0;
    size_t curr_to = 0;
    int64_t curr_weight = 0;
    std::cin >> curr_from >> curr_to >> curr_weight;
    graph.AddEdge(curr_from - 1, curr_to - 1, curr_weight);
  }
  auto min_distances = graph.MinDistances();
  for (auto& elem : min_distances) {
    std::cout << elem << ' ';
  }
}