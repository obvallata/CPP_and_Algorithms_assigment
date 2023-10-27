#include <algorithm>
#include <iostream>
#include <queue>

class Graph {
 public:
  explicit Graph(size_t am_ver) : edges_(am_ver), parent_(am_ver, -1), dist_(am_ver, -1){};
  void AddEdge(size_t from, size_t to) {
    edges_[from].emplace_back(to);
  }

  void PrintFindShortestWay(size_t, size_t);

 private:
  void BFS(size_t ver);

  std::vector<std::vector<size_t>> edges_;
  std::vector<int> parent_;
  std::vector<int> dist_;
};

int main() {
  size_t amount_ver = 0;
  size_t amount_edg = 0;
  size_t start_way = 0;
  size_t finish_way = 0;
  size_t curr_edg_start = 0;
  size_t curr_edg_finish = 0;
  std::cin >> amount_ver >> amount_edg >> start_way >> finish_way;
  Graph graph(amount_ver);
  for (size_t i = 0; i < amount_edg; ++i) {
    std::cin >> curr_edg_start >> curr_edg_finish;
    graph.AddEdge(curr_edg_start - 1, curr_edg_finish - 1);
    graph.AddEdge(curr_edg_finish - 1, curr_edg_start - 1);
  }
  graph.PrintFindShortestWay(start_way - 1, finish_way - 1);
  return 0;
}

void Graph::BFS(size_t ver) {
  dist_[ver] = 0;
  parent_[ver] = -1;
  std::queue<size_t> children;
  children.emplace(ver);
  while (!children.empty()) {
    auto curr_ver = children.front();
    children.pop();
    for (auto& neighbour : edges_[curr_ver]) {
      if (dist_[neighbour] == -1) {
        dist_[neighbour] = dist_[curr_ver] + 1;
        parent_[neighbour] = static_cast<int>(curr_ver);
        children.push(neighbour);
      }
    }
  }
}

void Graph::PrintFindShortestWay(size_t start, size_t finish) {
  BFS(start);
  if (dist_[finish] == -1) {
    std::cout << -1;
  } else {
    std::cout << dist_[finish] << '\n';
    std::vector<int> path;
    size_t current = finish;
    while (parent_[current] != -1) {
      path.emplace_back(current);
      current = parent_[current];
    }
    path.emplace_back(current);
    std::reverse(path.begin(), path.end());
    for (auto& elem : path) {
      std::cout << elem + 1 << ' ';
    }
  }
}