#include <algorithm>
#include <iostream>
#include <optional>
#include <stack>
#include <vector>

// TODO

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

  std::vector<EdgeId> GetRoutes() {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        DfsImpl(n);
      }
      n++;
    }
    return routes_;
  }

  void DfsImpl(VertexId vertex_id,
               std::optional<VertexId> parent_id = std::nullopt) {}

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<EdgeId> routes_;
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
  auto routes = router.GetRoutes();
  std::sort(routes.begin(), routes.end());
  //  routes.erase(std::unique(routes.begin(), routes.end()), routes.end());
  std::cout << routes.size() << '\n';
  for (const auto route : routes) {
    std::cout << route << ' ';
  }
}
