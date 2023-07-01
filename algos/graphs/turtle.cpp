#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

// short path on not-weight graph

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

template <typename Weight>
class DirectedWeightedGraph {
 private:
  using IncidenceList = std::vector<EdgeId>;
  using IncidentEdgesRange = Range<typename IncidenceList::const_iterator>;

 public:
  explicit DirectedWeightedGraph(size_t vertex_count = 0)
      : vertexes_(vertex_count + 1) {}

  EdgeId AddEdge(const Edge<Weight>& edge) {
    edges_.push_back(edge);
    const EdgeId id = edges_.size() - 1;
    vertexes_[edge.from].push_back(id);
    return id;
  }

  size_t GetVertexCount() const { return vertexes_.size() - 1; }

  size_t GetEdgeCount() const { return edges_.size(); }

  const Edge<Weight>& GetEdge(EdgeId edge_id) const { return edges_[edge_id]; }

  IncidentEdgesRange GetIncidentEdges(VertexId vertex) const {
    const auto& edges = vertexes_[vertex];
    return {std::begin(edges), std::end(edges)};
  }

 private:
  std::vector<Edge<Weight>> edges_;
  std::vector<IncidenceList> vertexes_;
};

struct Unit {};

using UnitGraph = DirectedWeightedGraph<Unit>;

struct ShortestPathInfo {
  int path;
  std::vector<size_t> vertexes;
};

template <typename Weight>
class Router {
 private:
  using Graph = DirectedWeightedGraph<Weight>;

  struct Used {
    bool is_used_{false};
  };

  struct Dist {
    int dist{-1};
  };

 public:
  explicit Router(const Graph& graph)
      : graph_(graph),
        dists_(graph.GetVertexCount() + 1),
        vertexes_used_(graph.GetVertexCount() + 1) {}

  ShortestPathInfo MinNotWeightPathFromTo(size_t from, size_t to) {
    const size_t vertexes_count = graph_.GetVertexCount();
    if (from > vertexes_count or to > vertexes_count) {
      return {-1, {}};
    } else if (from == to) {
      return {0, {from}};
    }
    std::queue<size_t> bfs_q;
    std::vector<size_t> path(vertexes_count + 1);
    path[from] = 0;
    dists_[from].dist = 0;
    bfs_q.push(from);
    while (!bfs_q.empty()) {
      size_t cur = bfs_q.front();
      bfs_q.pop();
      for (auto edg_id : graph_.GetIncidentEdges(cur)) {
        auto cur_to = graph_.GetEdge(edg_id).to;
        if (dists_[cur_to].dist == -1) {
          dists_[cur_to].dist = dists_[cur].dist + 1;
          bfs_q.push(cur_to);
          path[cur_to] = cur;
        }
      }
    }
    std::vector<size_t> real_path;
    for (size_t ver_id = to; ver_id != 0; ver_id = path[ver_id]) {
      real_path.push_back(ver_id);
    }
    std::reverse(real_path.begin(), real_path.end());
    return {dists_[to].dist, std::move(real_path)};
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<Dist> dists_;
};

}  // namespace graphs

int main() {
  size_t vertexes = 0, edges = 0;
  std::cin >> vertexes >> edges;
  graphs::UnitGraph graph(vertexes);
  size_t from = 0, to = 0;
  std::cin >> from >> to;
  for (size_t i = 0; i < edges; ++i) {
    size_t begin = 0, end = 0;
    std::cin >> begin >> end;
    graph.AddEdge({begin, end});
    graph.AddEdge({end, begin});
  }
  graphs::Router router(graph);
  auto info = router.MinNotWeightPathFromTo(from, to);
  std::cout << info.path << std::endl;
  if (info.path != -1) {
    for (auto p : info.vertexes) {
      std::cout << p << " ";
    }
  }
}
