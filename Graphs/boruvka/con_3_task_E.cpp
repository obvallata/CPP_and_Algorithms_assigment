#include <iostream>
#include <map>
#include <vector>

class DSU {
 public:
  explicit DSU(size_t amount_elems)
      : size_union_(amount_elems), parent_(amount_elems), amount_unions_(static_cast<int>(amount_elems)) {
    for (int i = 0; i < static_cast<int>(amount_elems); ++i) {
      MakeSet(i);
    }
  };

  void MakeSet(int elem) {
    parent_[elem] = elem;
    size_union_[elem] = 1;
  }

  int FindSet(int elem) {
    if (elem == parent_[elem]) {
      return elem;
    }
    return parent_[elem] = FindSet(parent_[elem]);
  }

  int UnionSets(int, int);
  size_t GetAmount() const {
    return amount_unions_;
  }

 private:
  std::vector<int> size_union_;
  std::vector<int> parent_;
  int amount_unions_ = 0;
};

class UndirectedGraph {
 public:
  explicit UndirectedGraph(size_t amount_ver, size_t amount_edge)
      : amount_ver_(amount_ver), edges_(amount_edge), unions_(amount_ver){};
  void AddEdge(size_t fir_ver, size_t sec_ver, size_t weight, size_t index) {
    edges_[index] = Edge{fir_ver, sec_ver, weight};
  }

  size_t MSTWeight() {
    return Boruvka();
  }

 private:
  size_t Boruvka();
  struct Edge {
    size_t from_ = 0;
    size_t to_ = 0;
    size_t weight_ = 0;
    Edge(size_t f, size_t t, size_t w) : from_(f), to_(t), weight_(w){};
    Edge() = default;
  };
  struct EdgeComparator {
    bool operator()(const Edge& fir_edge, const Edge& sec_edge) const {
      if (fir_edge.to_ == sec_edge.to_) {
        return fir_edge.from_ < sec_edge.from_;
      }
      return fir_edge.to_ < sec_edge.to_;
    }
  };
  const size_t k_max_weight_ = INT32_MAX;
  size_t amount_ver_ = 0;
  std::vector<Edge> edges_;
  DSU unions_;
};

int main() {
  std::ios::sync_with_stdio(false);
  size_t amount_ver = 0;
  size_t amount_edg = 0;
  std::cin >> amount_ver >> amount_edg;
  UndirectedGraph graph(amount_ver, amount_edg);
  for (size_t i = 0; i < amount_edg; ++i) {
    int curr_edg_start = 0;
    int curr_edg_finish = 0;
    int curr_weight = 0;
    std::cin >> curr_edg_start >> curr_edg_finish >> curr_weight;
    graph.AddEdge(curr_edg_start - 1, curr_edg_finish - 1, curr_weight, i);
  }
  std::cout << graph.MSTWeight();
  return 0;
}

int DSU::UnionSets(int fir_elem, int sec_elem) {
  fir_elem = FindSet(fir_elem);
  sec_elem = FindSet(sec_elem);
  if (sec_elem != fir_elem) {
    if (size_union_[sec_elem] > size_union_[fir_elem]) {
      parent_[fir_elem] = sec_elem;
      size_union_[sec_elem] += size_union_[fir_elem];
      --amount_unions_;
    } else {
      parent_[sec_elem] = fir_elem;
      size_union_[fir_elem] += size_union_[sec_elem];
      --amount_unions_;
    }
  }
  return std::max(size_union_[sec_elem], size_union_[fir_elem]);
}

size_t UndirectedGraph::Boruvka() {
  std::map<Edge, size_t, EdgeComparator> mst;
  while (unions_.GetAmount() > 1) {
    std::vector<Edge> min_edge(amount_ver_, {0, 0, k_max_weight_});
    for (auto& edge : edges_) {
      int union_from = unions_.FindSet(static_cast<int>(edge.from_));
      int union_to = unions_.FindSet(static_cast<int>(edge.to_));
      if (union_from != union_to) {
        if (edge.weight_ < min_edge[union_from].weight_) {
          min_edge[union_from] = edge;
        }
        if (edge.weight_ < min_edge[union_to].weight_) {
          min_edge[union_to] = edge;
        }
      }
    }
    for (auto& edge : min_edge) {
      if (edge.weight_ != k_max_weight_) {
        mst[Edge{std::min(edge.from_, edge.to_), std::max(edge.from_, edge.to_), 0}] = edge.weight_;
        unions_.UnionSets(static_cast<int>(edge.from_), static_cast<int>(edge.to_));
      }
    }
  }
  size_t weight_mst = 0;
  for (auto& edge : mst) {
    weight_mst += edge.second;
  }
  return weight_mst;
}