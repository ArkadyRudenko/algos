#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>
#include <cstdint>

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

  using RouteId = uint64_t;

  struct RouteInfo {
    RouteId id;
    Weight weight;
    size_t edge_count;
  };

  struct RouteInternalData {
    Weight weight;
    std::optional<EdgeId> prev_edge;
  };
  using RoutesInternalData =
      std::vector<std::vector<std::optional<RouteInternalData>>>;

  using ExpandedRoute = std::vector<EdgeId>;

 public:
  explicit Router(const Graph& graph)
      : graph_(graph),
        routes_internal_data_(graph.GetVertexCount(),
                              std::vector<std::optional<RouteInternalData>>(
                                  graph.GetVertexCount())) {
    InitializeRoutesInternalData(graph);

    const size_t vertex_count = graph.GetVertexCount();
    for (VertexId vertex_through = 0; vertex_through < vertex_count;
         ++vertex_through) {
      RelaxRoutesInternalDataThroughVertex(vertex_count, vertex_through);
    }
  }

  void InitializeRoutesInternalData(const Graph& graph) {
    const size_t vertex_count = graph.GetVertexCount();
    for (VertexId vertex = 0; vertex < vertex_count; ++vertex) {
      routes_internal_data_[vertex][vertex] =
          RouteInternalData{0, std::nullopt};
      for (const EdgeId edge_id : graph.GetIncidentEdges(vertex)) {
        const auto& edge = graph.GetEdge(edge_id);
        assert(edge.weight >= 0);
        auto& route_internal_data = routes_internal_data_[vertex][edge.to];
        if (!route_internal_data || route_internal_data->weight > edge.weight) {
          route_internal_data = RouteInternalData{edge.weight, edge_id};
        }
      }
    }
  }

  void RelaxRoutesInternalDataThroughVertex(size_t vertex_count,
                                            VertexId vertex_through) {
    for (VertexId vertex_from = 0; vertex_from < vertex_count; ++vertex_from) {
      if (const auto& route_from =
              routes_internal_data_[vertex_from][vertex_through]) {
        for (VertexId vertex_to = 0; vertex_to < vertex_count; ++vertex_to) {
          if (const auto& route_to =
                  routes_internal_data_[vertex_through][vertex_to]) {
            RelaxRoute(vertex_from, vertex_to, *route_from, *route_to);
          }
        }
      }
    }
  }

  void RelaxRoute(VertexId vertex_from, VertexId vertex_to,
                  const RouteInternalData& route_from,
                  const RouteInternalData& route_to) {
    auto& route_relaxing = routes_internal_data_[vertex_from][vertex_to];
    const Weight candidate_weight = route_from.weight + route_to.weight;
    if (!route_relaxing || candidate_weight < route_relaxing->weight) {
      route_relaxing = {candidate_weight, route_to.prev_edge
                                              ? route_to.prev_edge
                                              : route_from.prev_edge};
    }
  }

  std::vector<size_t> GetMinPathFromTo(VertexId from) {
    std::vector<size_t> res;
    res.reserve(graph_.GetVertexCount());
    for (size_t vertex = 0; vertex < graph_.GetVertexCount(); ++vertex) {
      if (vertex != from) {
        const auto to = vertex;
        const auto& route_internal_data = routes_internal_data_[from][to];
        if (!route_internal_data) {
          res.push_back(2009000999);
        } else {
          res.push_back(route_internal_data->weight);
        }
      } else {
        res.push_back(0);
      }
    }
    return res;
  }

 private:
  const Graph& graph_;

  mutable RouteId next_route_id_ = 0;
  mutable std::unordered_map<RouteId, ExpandedRoute> expanded_routes_cache_;

  RoutesInternalData routes_internal_data_;
};

}  // namespace graphs

int main() {
  size_t vertexes = 0, edges = 0;
  size_t maps = 0;
  std::cin >> maps;
  for (size_t map = 0; map < maps; ++map) {
    std::cin >> vertexes >> edges;
    graphs::DirectedWeightedGraph<size_t> graph(vertexes);
    for (size_t i = 0; i < edges; ++i) {
      size_t begin = 0, end = 0;
      size_t weight = 0;
      std::cin >> begin >> end >> weight;
      graph.AddEdge({begin, end, weight});
      graph.AddEdge({end, begin, weight});
    }
    graphs::Router router(graph);
    size_t from = 0;
    std::cin >> from;
    for (const auto path : router.GetMinPathFromTo(from)) {
      std::cout << path << " ";
    }
  }
}
