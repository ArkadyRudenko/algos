#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <stack>
#include <vector>

// Dijkstra algorithm

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

template <typename Weight>
class Router {
 private:
  using Graph = DirectedWeightedGraph<Weight>;
  using VertexList = std::vector<VertexId>;

  struct Used {
    bool is_used_{false};
  };

  enum class Color {
    White = 0,
    Grow = 1,
    Black = 2,
  };

 public:
  explicit Router(const Graph& graph)
      : graph_(graph),
        ready_dist_(graph.GetVertexCount() + 1, -1),
        dist_(graph.GetVertexCount() + 1, -1),
        vertexes_used_(graph.GetVertexCount() + 1) {}

  size_t GetMinPathFromTo(VertexId from, VertexId to) {
    ready_dist_[from] = dist_[from] = 0;
    DfsImpl(from);
    return ready_dist_[to];
  }

  void DfsImpl(VertexId vertex_id, VertexId parent = 0) {
    vertexes_used_[vertex_id].is_used_ = true;
    Relax(vertex_id);
    UpdateCut(vertex_id, parent);
    if (cut_.empty()) {
      return;
    }
    const auto& edge = GetMinEdgeFromCut();
    ready_dist_[edge.to] = dist_[edge.to];
    DfsImpl(edge.to, vertex_id);
  }

  void Relax(VertexId ver) {
    for (auto edg_id : graph_.GetIncidentEdges(ver)) {
      const auto& edge = graph_.GetEdge(edg_id);
      if (dist_[edge.to] == -1) {
        dist_[edge.to] = ready_dist_[ver] + edge.weight;
      }
      dist_[edge.to] = std::min(dist_[edge.to], ready_dist_[ver] + edge.weight);
    }
  }

  void UpdateCut(VertexId ver, VertexId parent) {
    for (auto edg_id : graph_.GetIncidentEdges(ver)) {
      const auto& edge = graph_.GetEdge(edg_id);
      if (!vertexes_used_[edge.to].is_used_ && edge.to != parent) {
        cut_.insert(edge);
      }
    }
  }

  Edge<Weight> GetMinEdgeFromCut() {
    const auto res = *cut_.begin();
    cut_.erase(res);
    return res;
  }

 private:
  const Graph& graph_;
  std::vector<int64_t> ready_dist_;
  std::vector<int64_t> dist_;
  std::vector<Used> vertexes_used_;
  std::set<Edge<Weight>, decltype([](const Edge<Weight>& lhs,
                                     const Edge<Weight> rhs) -> bool {
             return lhs.weight < rhs.weight;
           })>
      cut_;
};

}  // namespace graphs

int main() {
  size_t vertexes = 0, edges = 0;
  std::cin >> vertexes >> edges;
  graphs::DirectedWeightedGraph<int64_t> graph(vertexes);
  size_t from = 0, to = 0;
  std::cin >> from >> to;
  for (size_t i = 0; i < edges; ++i) {
    size_t begin = 0, end = 0;
    int64_t weight = 0;
    std::cin >> begin >> end >> weight;
    graph.AddEdge({begin, end, weight});
    graph.AddEdge({end, begin, weight});
  }
  graphs::Router router(graph);
  std::cout << router.GetMinPathFromTo(from, to);
}
