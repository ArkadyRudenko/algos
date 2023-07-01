#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

namespace graphs {

template <typename It>
class Range {
 public:
  using ValueType = typename std::iterator_traits<It>::value_type;

  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }

 private:
  It begin_;
  It end_;
};

using VertexId = size_t;
using EdgeId = size_t;

template <typename Weight>
struct Edge {
  VertexId from;
  VertexId to;
  Weight weight;
};

struct Unit {};

template <typename Weight>
class DirectedWeightedGraph {
 private:
  using IncidenceList = std::vector<EdgeId>;
  using IncidentEdgesRange = Range<typename IncidenceList::const_iterator>;

  struct Used {
    bool is_used_{false};
  };

  struct Dist {
    int dist{-1};
  };

 public:
  explicit DirectedWeightedGraph(size_t vertex_count = 0)
      : vertexes_(vertex_count + 1),
        vertexes_used_(vertex_count + 1),
        dists_(vertex_count + 1) {}

  EdgeId AddEdge(const Edge<Weight>& edge) {
    edges_.push_back(edge);
    const EdgeId id = edges_.size() - 1;
    vertexes_[edge.from].push_back(id);
    return id;
  }

  size_t GetVertexCount() const { return vertexes_.size(); }

  size_t GetEdgeCount() const { return edges_.size(); }

  const Edge<Weight>& GetEdge(EdgeId edge_id) const { return edges_[edge_id]; }

  IncidentEdgesRange GetIncidentEdges(VertexId vertex) const {
    const auto& edges = vertexes_[vertex];
    return {std::begin(edges), std::end(edges)};
  }

  size_t GetMinRoute(size_t begin_id, size_t end_id) const { return 0; }

  // This Dfs only for not directed not weight
  size_t Dfs() const {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        DfsImpl(n);
      }
      n++;
    }
    vertexes_used_ = std::vector<Used>(vertexes_.size());
    return 0;
  }

  size_t DfsImpl(size_t vertex_id) const {
    vertexes_used_[vertex_id].is_used_ = true;
    std::cout << "VertexId = " << vertex_id << std::endl;
    for (auto edg_id : vertexes_[vertex_id]) {
      auto ver_id = edges_[edg_id].to;
      if (!vertexes_used_[ver_id].is_used_) {
        DfsImpl(ver_id);
      }
    }
    return 0;
  }

  void DfsNotRecursion() const {
    std::stack<VertexId> vertexes;
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        vertexes.push(n);
        while (!vertexes.empty()) {
          auto ver_id = vertexes.top();
          vertexes_used_[ver_id].is_used_ = true;
          std::cout << "VertexId = " << ver_id << std::endl;
          vertexes.pop();
          for (auto edg_id : vertexes_[ver_id]) {
            vertexes.push(edges_[edg_id].to);
          }
        }
      }
      n++;
    }
    vertexes_used_ = std::vector<Used>(vertexes_.size());
  }

  void Bfs() const {
    for (size_t ver_id{0}; auto& dist : dists_) {
      if (ver_id != 0 && dist.dist == -1) {
        BfsImpl(ver_id);
      }
      ver_id++;
    }
    dists_ = std::vector<Dist>(dists_.size());
  }

  void BfsImpl(size_t v, size_t t) const {
    if (v > vertexes_.size() - 1 || t > vertexes_.size() - 1) {
      std::cout << "-1" << std::endl;
    }
    if (v == t) {
      std::cout << 0 << std::endl;
      std::cout << v << std::endl;
      return;
    }
    std::queue<size_t> bfs_q;
    std::vector<size_t> path(vertexes_.size() + 1);
    path[v] = 0;
    dists_[v].dist = 0;
    bfs_q.push(v);
    while (!bfs_q.empty()) {
      size_t cur = bfs_q.front();
      bfs_q.pop();
      for (auto edg_id : vertexes_[cur]) {
        auto to = edges_[edg_id].to;
        if (dists_[to].dist == -1) {
          dists_[to].dist = dists_[cur].dist + 1;
          bfs_q.push(to);
          path[to] = cur;
        }
      }
    }
    std::cout << dists_[t].dist << std::endl;
    if (dists_[t].dist != -1) {
      std::vector<size_t> real_path;
      for (size_t ver_id = t; ver_id != 0; ver_id = path[ver_id]) {
        real_path.push_back(ver_id);
      }
      std::reverse(real_path.begin(), real_path.end());
      for (auto p : real_path) {
        std::cout << p << " ";
      }
    }
  }

 private:
  std::vector<Edge<Weight>> edges_;
  std::vector<IncidenceList> vertexes_;
  mutable std::vector<Used> vertexes_used_;
  mutable std::vector<Dist> dists_;
};

using UnitGraph = DirectedWeightedGraph<Unit>;

}  // namespace graphs

int main() {
  size_t vertexes = 0, edges = 0;
  std::cin >> vertexes >> edges;
  graphs::UnitGraph graph(vertexes);
  size_t s = 0, t = 0;
  std::cin >> s >> t;
  for (size_t i = 0; i < edges; ++i) {
    size_t begin = 0, end = 0;
    std::cin >> begin >> end;
    graph.AddEdge({begin, end});
    graph.AddEdge({end, begin});
  }
  graph.BfsImpl(s, t);
}
