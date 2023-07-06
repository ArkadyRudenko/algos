#include <algorithm>
#include <iostream>
#include <optional>
#include <stack>
#include <vector>

#include <gtest/gtest.h>

// Bridges

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
  size_t real_id;
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
using UnitEdge = Edge<Unit>;

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
        vertexes_used_(graph.GetVertexCount() + 1),
        ret(graph.GetVertexCount() + 1),
        times_(graph.GetVertexCount() + 1) {}

  std::vector<EdgeId> GetBridges() {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        DfsImpl(n);
      }
      n++;
    }
    return bridges;
  }

  void DfsImpl(VertexId vertex_id,
               std::optional<VertexId> parent_id = std::nullopt) {
    vertexes_used_[vertex_id].is_used_ = true;
    times_[vertex_id] = ret[vertex_id] = time_++;
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      if (IsParentVertex(edge, parent_id) || IsSelfEdge(edge)) {
        continue;
      }
      const auto to = edge.to;
      if (vertexes_used_[to].is_used_) {
        ret[vertex_id] = std::min(ret[vertex_id], times_[to]);
      } else {
        DfsImpl(to, vertex_id);
        ret[vertex_id] = std::min(ret[vertex_id], ret[to]);
        if (ret[to] > times_[vertex_id]) {
          PushBridge(edg_id);
        }
      }
    }
    // (v, t) <=> t_in(t) == ret(t)
  }

  void PushBridge(const EdgeId edg_id) {
    const auto& edge = graph_.GetEdge(edg_id);
    if (IsNotMulti(edge.from, edge.to) && IsNotMulti(edge.to, edge.from)) {
      bridges.push_back(edg_id);
    }
  }

  bool IsNotMulti(VertexId from, VertexId to) {
    size_t multi_count{0};
    for (auto cur_edg_id : graph_.GetIncidentEdges(from)) {
      const auto& cur_edge = graph_.GetEdge(cur_edg_id);
      if (cur_edge.to == to) {
        ++multi_count;
      }
    }
    return multi_count == 1;
  }

  bool IsSelfEdge(const Edge<Weight>& edge) { return edge.to == edge.from; }

  bool IsParentVertex(const Edge<Weight>& edge,
                      std::optional<VertexId> parent_id) {
    if (parent_id.has_value()) {
      const auto parent = parent_id.value();
      return parent == edge.to;
    }
    return false;
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<size_t> ret;
  std::vector<size_t> times_;
  size_t time_{0};
  std::vector<EdgeId> bridges;
};

}  // namespace graphs


TEST(Graphs, Bridges) {
  std::vector<graphs::UnitEdge> edges = {
      {1, 2},
      {2, 3},
      {3, 4},
      {1, 3},
//      {4, 5},
      {4, 6},
      {5, 6},
  };
  graphs::UnitGraph graph(6);
  for (size_t n{1}; const auto& e : edges) {
    const auto from = e.from;
    const auto to = e.to;
    if (from != to) {
      graph.AddEdge({from, to, n});
      graph.AddEdge({to, from, n});
    } else {
      graph.AddEdge({from, to, n});
    }
    ++n;
  }
  graphs::Router router(graph);
  auto bridges = router.GetBridges();
  std::sort(bridges.begin(), bridges.end());
  std::cout << bridges.size() << '\n';
  for (const auto bridge : bridges) {
    std::cout << graph.GetEdge(bridge).real_id << ' ';
  }
}