#include <algorithm>
#include <iostream>
#include <vector>

class DirectedGraph {
 public:
  explicit DirectedGraph(uint16_t amount_ver) : amount_vertexes_(amount_ver) {
    edges_.reserve(amount_ver);
  };

  void AddEdge(uint16_t from, uint16_t to, int32_t weight) {
    edges_.emplace_back(from, to, weight);
  }

  std::vector<uint16_t> NegCycle() {
    return FordBellmanNegCycle();
  }

 private:
  struct Edge {
    uint16_t from_ = 0;
    uint16_t to_ = 0;
    int32_t weight_ = 0;
    Edge(uint16_t from, uint16_t to, int32_t weight) : from_(from), to_(to), weight_(weight){};
    Edge() = default;
  };

  static bool RelaxEdge(const Edge& edge, std::vector<int32_t>& dist, std::vector<uint16_t>& parent);
  std::vector<uint16_t> FordBellmanNegCycle(uint16_t begin = 0);

  uint16_t amount_vertexes_ = 0;
  std::vector<Edge> edges_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  const int32_t k_no_edge = 100000;
  uint16_t amount_vertexes = 0;
  std::cin >> amount_vertexes;
  DirectedGraph graph(amount_vertexes);
  for (uint16_t i = 0; i < amount_vertexes; ++i) {
    for (uint16_t j = 0; j < amount_vertexes; ++j) {
      int32_t curr_weight = 0;
      std::cin >> curr_weight;
      if (curr_weight != k_no_edge) {
        graph.AddEdge(i, j, curr_weight);
      }
    }
  }
  auto neg_cycle = graph.NegCycle();
  if (neg_cycle.empty()) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n";
    std::cout << neg_cycle.size() << '\n';
    for (uint16_t i = neg_cycle.size() - 1; i > 0; --i) {
      std::cout << neg_cycle[i] << ' ';
    }
    std::cout << neg_cycle[0] << ' ';
  }
  return 0;
}

std::vector<uint16_t> DirectedGraph::FordBellmanNegCycle(uint16_t begin) {
  std::vector<uint16_t> neg_cycle;
  std::vector<uint16_t> parent(amount_vertexes_);
  std::vector<int32_t> dist(amount_vertexes_, 0);
  dist[begin] = 0;
  for (uint16_t i = 0; i < amount_vertexes_ - 1; ++i) {
    for (auto& edge : edges_) {
      RelaxEdge(edge, dist, parent);
    }
  }
  bool neg_cycle_exists = false;
  bool relax_happened = false;
  uint16_t curr_ver = 0;
  for (auto& edge : edges_) {
    relax_happened = RelaxEdge(edge, dist, parent);
    if (relax_happened) {
      curr_ver = edge.to_;
      neg_cycle_exists = true;
    }
  }
  if (neg_cycle_exists) {
    for (uint16_t i = 0; i < amount_vertexes_; ++i) {
      curr_ver = parent[curr_ver];
    }
    uint16_t start_cycle = curr_ver;
    do {
      neg_cycle.emplace_back(curr_ver + 1);
      curr_ver = parent[curr_ver];
    } while (curr_ver != start_cycle);
  }
  if (!neg_cycle.empty()) {
    neg_cycle.emplace_back(neg_cycle[0]);
  }
  return neg_cycle;
}

bool DirectedGraph::RelaxEdge(const DirectedGraph::Edge& edge, std::vector<int32_t>& dist,
                              std::vector<uint16_t>& parent) {
  if (dist[edge.to_] > dist[edge.from_] + edge.weight_) {
    dist[edge.to_] = dist[edge.from_] + edge.weight_;
    parent[edge.to_] = edge.from_;
    return true;
  }
  return false;
}