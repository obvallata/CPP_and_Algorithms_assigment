#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

class Graph {
 public:
  explicit Graph(size_t am_ver) : edges_(am_ver), dist_(am_ver, kMaxWeight), prev_(am_ver, -1){};
  void AddEdge(size_t, size_t, size_t);
  size_t MSTWeight() {
    return Prim();
  }

 private:
  size_t Prim();
  struct Edge {
    size_t from_ = 0;
    size_t to_ = 0;
  };
  struct EdgeComparator {
    bool operator()(const Edge &fir, const Edge &sec) const {
      if (fir.from_ == sec.from_) {
        return fir.to_ < sec.to_;
      }
      return fir.from_ < sec.from_;
    }
  };
  struct Dest {
    size_t id_ = 0;
    size_t dist_ = kMaxWeight;
  };
  struct DestComparator {
    bool operator()(const Dest &fir, const Dest &sec) const {
      if (fir.dist_ == sec.dist_) {
        return fir.id_ > sec.id_;
      }
      return fir.dist_ > sec.dist_;
    }
  };

  static Edge RightOrderedEdge(const size_t &fir_ver, const size_t &sec_ver) {
    if (fir_ver < sec_ver) {
      return Edge{fir_ver, sec_ver};
    }
    return Edge{sec_ver, fir_ver};
  }

  static const int kMaxWeight = 10e7;
  std::vector<std::vector<size_t>> edges_;
  std::vector<size_t> dist_;
  std::vector<int> prev_;
  std::map<Edge, size_t, EdgeComparator> weight_;
};

int main() {
  size_t amount_ver = 0;
  size_t amount_edg = 0;
  std::cin >> amount_ver >> amount_edg;
  Graph graph(amount_ver);
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

void Graph::AddEdge(size_t ver_f, size_t ver_s, size_t weight) {
  edges_[ver_f].emplace_back(ver_s);
  edges_[ver_s].emplace_back(ver_f);
  if (weight_.find(RightOrderedEdge(ver_f, ver_s)) != weight_.end()) {
    weight_[RightOrderedEdge(ver_f, ver_s)] = std::min(weight, weight_[RightOrderedEdge(ver_f, ver_s)]);
  } else {
    weight_[RightOrderedEdge(ver_f, ver_s)] = weight;
  }
}

size_t Graph::Prim() {
  dist_[0] = 0;
  std::set<Edge, EdgeComparator> weight_mst;
  std::vector<bool> used(dist_.size(), false);
  std::priority_queue<Dest, std::vector<Dest>, DestComparator> min_dist_queue;
  for (size_t i = 0; i < edges_.size(); ++i) {
    min_dist_queue.emplace(Dest{i, dist_[i]});
  }
  while (!min_dist_queue.empty()) {
    auto curr_edge = min_dist_queue.top();
    min_dist_queue.pop();
    if (dist_[curr_edge.id_] != curr_edge.dist_) {
      continue;
    }
    if (prev_[curr_edge.id_] != -1) {
      weight_mst.insert(RightOrderedEdge(prev_[curr_edge.id_], curr_edge.id_));
    }
    used[curr_edge.id_] = true;
    for (auto &elem : edges_[curr_edge.id_]) {
      if (!used[elem] && dist_[elem] > weight_[RightOrderedEdge(elem, curr_edge.id_)]) {
        prev_[elem] = static_cast<int>(curr_edge.id_);
        dist_[elem] = weight_[RightOrderedEdge(elem, curr_edge.id_)];
        min_dist_queue.push(Dest{elem, dist_[elem]});
      }
    }
  }
  size_t sum_weight = 0;
  for (auto &edge : weight_mst) {
    sum_weight += weight_[edge];
  }
  return sum_weight;
}