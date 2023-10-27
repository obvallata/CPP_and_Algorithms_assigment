#include <iostream>
#include <vector>

class DSU {
 public:
  explicit DSU(size_t amount_elems)
      : size_union_(amount_elems)
      , parent_(amount_elems)
      , weight_(amount_elems)
      , amount_unions_(static_cast<int>(amount_elems)) {
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

  int UnionSets(int, int, size_t);

  size_t GetWeight(int ver) const {
    return weight_[ver];
  }

 private:
  std::vector<int> size_union_;
  std::vector<int> parent_;
  std::vector<size_t> weight_;
  int amount_unions_ = 0;
};

class UndirectedGraph {
 public:
  explicit UndirectedGraph(size_t amount_ver) : unions_(amount_ver){};
  void AddEdge(size_t fir_ver, size_t sec_ver, size_t weight) {
    unions_.UnionSets(static_cast<int>(fir_ver), static_cast<int>(sec_ver), weight);
  }

  size_t ComponentWeight(size_t ver) {
    return unions_.GetWeight(unions_.FindSet(static_cast<int>(ver)));
  }

 private:
  DSU unions_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t amount_ver = 0;
  size_t amount_req = 0;
  std::cin >> amount_ver >> amount_req;
  UndirectedGraph graph(amount_ver);
  for (size_t i = 0; i < amount_req; ++i) {
    size_t request_num = 0;
    std::cin >> request_num;
    if (request_num == 1) {
      size_t curr_edg_start = 0;
      size_t curr_edg_finish = 0;
      size_t curr_weight = 0;
      std::cin >> curr_edg_start >> curr_edg_finish >> curr_weight;
      graph.AddEdge(curr_edg_start - 1, curr_edg_finish - 1, curr_weight);
    } else {
      size_t curr_ver = 0;
      std::cin >> curr_ver;
      std::cout << graph.ComponentWeight(curr_ver - 1) << '\n';
    }
  }
  return 0;
}

int DSU::UnionSets(int fir_elem, int sec_elem, size_t weight) {
  fir_elem = FindSet(fir_elem);
  sec_elem = FindSet(sec_elem);
  if (sec_elem != fir_elem) {
    if (size_union_[sec_elem] > size_union_[fir_elem]) {
      parent_[fir_elem] = sec_elem;
      size_union_[sec_elem] += size_union_[fir_elem];
      weight_[sec_elem] += weight_[fir_elem] + weight;
      --amount_unions_;
    } else {
      parent_[sec_elem] = fir_elem;
      size_union_[fir_elem] += size_union_[sec_elem];
      weight_[fir_elem] += weight_[sec_elem] + weight;
      --amount_unions_;
    }
  } else {
    weight_[fir_elem] += weight;
  }
  return std::max(size_union_[sec_elem], size_union_[fir_elem]);
}