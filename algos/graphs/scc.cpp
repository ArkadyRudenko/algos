#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

// Strongly connected components

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
      : graph_(graph), vertexes_used_(graph.GetVertexCount() + 1) {}

  std::vector<VertexId> FindTopSort() {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        DfsImplForTopSort(n);
      }
      n++;
    }
    std::reverse(sort_.begin(), sort_.end());
    return sort_;
  }

  void DfsImplForTopSort(const VertexId vertex_id) {
    vertexes_used_[vertex_id].is_used_ = true;
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      if (!vertexes_used_[edge.to].is_used_) {
        DfsImplForTopSort(edge.to);
      }
    }
    sort_.push_back(vertex_id);
  }

  void DfsImpl(const VertexId vertex_id) {
    vertexes_used_[vertex_id].is_used_ = true;
    group_.push_back(vertex_id);
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      const auto to = edge.to;
      if (!vertexes_used_[to].is_used_) {
        DfsImpl(to);
      }
    }
  }

  VertexList TakeGroup() { return std::move(group_); }

  bool IsUsed(VertexId vertex_id) const {
    return vertexes_used_[vertex_id].is_used_;
  }

  std::vector<VertexList> Scc() {
    const auto sort = FindTopSort();
    if (sort.empty()) {
      return {};
    }
    Graph reverse_graph(graph_.GetVertexCount());
    for (size_t i = 0; i < graph_.GetEdgeCount(); ++i) {
      const auto& edge = graph_.GetEdge(i);
      reverse_graph.AddEdge({edge.to, edge.from, edge.weight});
    }
    Router router(reverse_graph);
    std::vector<VertexList> groups;
    for (const auto ver_id : sort) {
      if (!router.IsUsed(ver_id)) {
        router.DfsImpl(ver_id);
        groups.push_back(router.TakeGroup());
      }
    }
    return groups;
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<VertexId> sort_;
  VertexList group_;
};

}  // namespace graphs

int main() {
  size_t vertexes = 0, edges = 0;
  std::cin >> vertexes >> edges;
  graphs::UnitGraph graph(vertexes);
  for (size_t i = 0; i < edges; ++i) {
    size_t begin = 0, end = 0;
    std::cin >> begin >> end;
    graph.AddEdge({begin, end});
  }
  graphs::Router router(graph);
  const auto groups = router.Scc();
  std::cout << groups.size() << '\n';
  std::vector<size_t> verts(graph.GetVertexCount() + 1);
  for (size_t group_id{1}; const auto& group : groups) {
    for (const auto drone : group) {
      verts[drone] = group_id;
    }
    ++group_id;
  }
  for (size_t n{0}; const auto v : verts) {
    if (n != 0) {
      std::cout << v << ' ';
    }
    ++n;
  }
}
