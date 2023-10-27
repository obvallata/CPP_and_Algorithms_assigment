#include <iostream>
#include <map>
#include <queue>
#include <set>

class UndirectedGraph {
 public:
  explicit UndirectedGraph(size_t am_ver) : edges_(am_ver){};
  void AddEdge(size_t, size_t, size_t);
  std::vector<size_t> FindAllDist(size_t);

 private:
  struct Dest {
    size_t to_;
    size_t weight_;
    Dest(size_t to, size_t weight) : to_(to), weight_(weight){};
  };

  struct Vertex {
    size_t id_;
    size_t dist_;
    Vertex(size_t id, size_t dist) : id_(id), dist_(dist){};
  };

  struct VertexComparator {
    bool operator()(const Vertex& from, const Vertex& to) const {
      if (from.dist_ == to.dist_) {
        return from.id_ < to.id_;
      }
      return from.dist_ < to.dist_;
    }
  };
  std::vector<size_t> Dijkstra(size_t);
  const size_t k_max_dist_ = 2009000999;
  std::vector<std::vector<Dest>> edges_;
};

int main() {
  size_t amount_graphs = 0;
  std::cin >> amount_graphs;
  for (size_t i = 0; i < amount_graphs; ++i) {
    size_t amount_ver = 0;
    size_t amount_edg = 0;
    std::cin >> amount_ver >> amount_edg;
    UndirectedGraph graph(amount_ver);
    for (size_t j = 0; j < amount_edg; ++j) {
      int curr_edg_start = 0;
      int curr_edg_finish = 0;
      int curr_weight = 0;
      std::cin >> curr_edg_start >> curr_edg_finish >> curr_weight;
      graph.AddEdge(curr_edg_start, curr_edg_finish, curr_weight);
    }
    size_t begin = 0;
    std::cin >> begin;
    auto distances = graph.FindAllDist(begin);
    for (auto& dist : distances) {
      std::cout << dist << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}

void UndirectedGraph::AddEdge(size_t ver_f, size_t ver_s, size_t weight) {
  edges_[ver_f].emplace_back(ver_s, weight);
  edges_[ver_s].emplace_back(ver_f, weight);
}

std::vector<size_t> UndirectedGraph::FindAllDist(size_t begin) {
  return Dijkstra(begin);
}

std::vector<size_t> UndirectedGraph::Dijkstra(size_t begin) {
  std::vector<size_t> dist(edges_.size(), k_max_dist_);
  dist[begin] = 0;
  std::set<Vertex, VertexComparator> inc_dists;
  for (size_t i = 0; i < edges_.size(); ++i) {
    inc_dists.emplace(i, dist[i]);
  }
  while (!inc_dists.empty()) {
    auto min_ver = *inc_dists.begin();
    inc_dists.erase(inc_dists.begin());
    for (auto& dest : edges_[min_ver.id_]) {
      if (inc_dists.find(Vertex{dest.to_, dist[dest.to_]}) != inc_dists.end() &&
          dist[dest.to_] > dist[min_ver.id_] + dest.weight_) {
        inc_dists.erase(Vertex{dest.to_, dist[dest.to_]});
        dist[dest.to_] = dist[min_ver.id_] + dest.weight_;
        inc_dists.emplace(dest.to_, dist[dest.to_]);
      }
    }
  }
  return dist;
}