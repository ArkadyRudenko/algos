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

 public:
  explicit Router(const Graph& graph)
      : graph_(graph), vertexes_used_(graph.GetVertexCount() + 1) {}

  std::vector<VertexList> Drones() {
    std::vector<VertexList> groups;
    for (size_t n{0}; auto used : vertexes_used_) {
      if (n != 0 and !used.is_used_) {
        DfsImpl(n);
        groups.push_back(group_);
        group_.clear();
      }
      ++n;
    }
    return groups;
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

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
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
    graph.AddEdge({end, begin});
  }
  graphs::Router router(graph);
  const auto groups = router.Drones();
  std::cout << groups.size() << '\n';
  for (const auto& group : groups) {
    if (!group.empty()) {
      std::cout << group.size() << '\n';
      for (const auto drone : group) {
        std::cout << drone << " ";
      }
      std::cout << '\n';
    }
  }
}
