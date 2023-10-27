#include <iostream>
#include <vector>
#include <iostream>

class DSU {
 public:
  explicit DSU(size_t amount_elems) : size_union_(amount_elems), parent_(amount_elems) {
    size_ += amount_elems;
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

  size_t GetSize() const {
    return size_;
  }

 private:
  std::vector<int> size_union_;
  std::vector<int> parent_;
  size_t size_ = 0;
};

class UndirectedGraph {
 public:
  explicit UndirectedGraph(size_t amount_ver, size_t amount_edge) : edges_(amount_edge), unions_(amount_ver){};

  void AddEdge(size_t fir_ver, size_t sec_ver, size_t weight) {
    edges_[amount_edges_++] = Edge{fir_ver, sec_ver, weight};
  }

  size_t BuiltBridges() {
    return KruskalModified();
  }

 private:
  size_t KruskalModified();
  struct Edge {
    size_t from;
    size_t to;
    size_t weight;
    Edge(size_t f, size_t t, size_t w) : from(f), to(t), weight(w){};
    Edge() = default;
  };
  std::vector<Edge> edges_;
  DSU unions_;
  size_t amount_edges_ = 0;
};

int main() {
  size_t amount_islands = 0;
  size_t amount_bridges = 0;
  std::cin >> amount_islands >> amount_bridges;
  UndirectedGraph graph(amount_islands, amount_bridges);
  for (size_t i = 0; i < amount_bridges; ++i) {
    size_t from = 0;
    size_t to = 0;
    std::cin >> from >> to;
    graph.AddEdge(from, to, 1);
  }
  std::cout << graph.BuiltBridges();
  return 0;
}

int DSU::UnionSets(int fir_elem, int sec_elem) {
  fir_elem = FindSet(fir_elem);
  sec_elem = FindSet(sec_elem);
  if (sec_elem != fir_elem) {
    if (size_union_[sec_elem] > size_union_[fir_elem]) {
      parent_[fir_elem] = sec_elem;
      size_union_[sec_elem] += size_union_[fir_elem];
      --size_;
    } else {
      parent_[sec_elem] = fir_elem;
      size_union_[fir_elem] += size_union_[sec_elem];
      --size_;
    }
  }
  return std::max(size_union_[sec_elem], size_union_[fir_elem]);
}

size_t UndirectedGraph::KruskalModified() {
  size_t amount_bridges = 0;
  for (auto& edge : edges_) {
    if (unions_.FindSet(static_cast<int>(edge.from)) != unions_.FindSet(static_cast<int>(edge.to))) {
      unions_.UnionSets(static_cast<int>(edge.from), static_cast<int>(edge.to));
    }
    amount_bridges += edge.weight;
    if (unions_.GetSize() == 1) {
      break;
    }
  }
  return amount_bridges;
}