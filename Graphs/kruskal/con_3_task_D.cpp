#include <vector>
#include <iostream>
class DSU {
 public:
  explicit DSU(size_t amount_elems) : size_union_(amount_elems), parent_(amount_elems) {
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

  int UnionSets(int fir_elem, int sec_elem);

 private:
  std::vector<int> size_union_;
  std::vector<int> parent_;
};

class UndirectedGraph {
 public:
  explicit UndirectedGraph(size_t amount_ver, size_t amount_edge) : edges_(amount_edge), unions_(amount_ver){};
  void AddEdge(size_t fir_ver, size_t sec_ver, size_t weight) {
    edges_.emplace_back(fir_ver, sec_ver, weight);
  }

  size_t MSTWeight() {
    return Kruskal();
  }

 private:
  size_t Kruskal();
  struct Edge {
    size_t from;
    size_t to;
    size_t weight;
    Edge(size_t f, size_t t, size_t w) : from(f), to(t), weight(w){};
    Edge() = default;
  };
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
    graph.AddEdge(curr_edg_start - 1, curr_edg_finish - 1, curr_weight);
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
    } else {
      parent_[sec_elem] = fir_elem;
      size_union_[fir_elem] += size_union_[sec_elem];
    }
  }
  return std::max(size_union_[sec_elem], size_union_[fir_elem]);
}

size_t UndirectedGraph::Kruskal() {
  size_t weight_mst = 0;
  for (auto& edge : edges_) {
    if (unions_.FindSet(static_cast<int>(edge.from)) != unions_.FindSet(static_cast<int>(edge.to))) {
      unions_.UnionSets(static_cast<int>(edge.from), static_cast<int>(edge.to));
      weight_mst += edge.weight;
    }
  }
  return weight_mst;
}