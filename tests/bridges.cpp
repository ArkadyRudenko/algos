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
        vertexes_used_(graph.GetVertexCount() + 1),
        ret_(graph.GetVertexCount() + 1),
        times_(graph.GetVertexCount() + 1) {}

  size_t GetBridges() {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        DfsImpl(n);
      }
      n++;
    }
    size_t res = bridges_.size();
    size_t prev_from{0};
    bool is_expired{false};
    for (size_t i = 0; i < bridges_.size() - 1; ++i) {
      const auto cur_edge = graph_.GetEdge(bridges_[i]);
      const auto next_edge = graph_.GetEdge(bridges_[i + 1]);
      if (cur_edge.from == next_edge.from) {
        res--;
      }
    }
    return res;
  }

  void DfsImpl(VertexId vertex_id,
               std::optional<VertexId> parent_id = std::nullopt) {
    vertexes_used_[vertex_id].is_used_ = true;
    times_[vertex_id] = ret_[vertex_id] = time_++;
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      if (IsParentVertex(edge, parent_id) || IsSelfEdge(edge)) {
        continue;
      }
      const auto to = edge.to;
      if (vertexes_used_[to].is_used_) {
        ret_[vertex_id] = std::min(ret_[vertex_id], times_[to]);
      } else {
        DfsImpl(to, vertex_id);
        ret_[vertex_id] = std::min(ret_[vertex_id], ret_[to]);
        if (ret_[to] > times_[vertex_id]) {
          PushBridge(edg_id);
        }
      }
    }
    // (v, t) <=> t_in(t) == ret(t)
  }

  void PushBridge(const EdgeId edg_id) {
    const auto& edge = graph_.GetEdge(edg_id);
    if (IsNotMulti(edge.from, edge.to) && IsNotMulti(edge.to, edge.from)) {
      bridges_.push_back(edg_id);
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

  size_t GetCountOfLoners(VertexId ver_id = 0) {
    if (ver_id == 0) {
      ver_id = 1;
    }
    vertexes_used_[ver_id].is_used_ = true;
    size_t children{0};
    for (auto edg_id : graph_.GetIncidentEdges(ver_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      children++;
      if (!vertexes_used_[edge.to].is_used_) {
        GetCountOfLoners(edge.to);
      }
    }
    if (children == 1) {
      count_++;
    }
    return count_;
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  size_t count_{0};
  std::vector<size_t> ret_;
  std::vector<size_t> times_;
  size_t time_{0};
  std::vector<EdgeId> bridges_;
};

using UnitEdge = Edge<Unit>;

}  // namespace graphs

TEST(Graphs, BridgesAdd1) {
  /*
   *       |
   *    -- * --
   *       |
   */
  std::vector<graphs::UnitEdge> edges = {
      {1, 2},
      {1, 3},
      {1, 4},
      {1, 5},
  };
  graphs::UnitGraph graph(5);
  for (const auto& e : edges) {
    graph.AddEdge(e);
    graph.AddEdge({e.to, e.from});
  }
  graphs::Router router(graph);
  ASSERT_EQ(2, router.GetBridges());
}

TEST(Graphs, BridgesAdd2) {
  /*
   *    *             *
   *     \           /
   *      * -- * -- *
   *     /           \
   *    *             *
  */
  std::vector<graphs::UnitEdge> edges = {
      {1, 2}, {1, 3}, {1, 4}, {4, 5}, {5, 6}, {5, 7},
  };
  graphs::UnitGraph graph(7);
  for (const auto& e : edges) {
    graph.AddEdge(e);
    graph.AddEdge({e.to, e.from});
  }
  graphs::Router router(graph);
  ASSERT_EQ(2, router.GetBridges());
}

TEST(Graphs, BridgesAdd3) {
  /*
   *    *             *
   *    |\           /|
   *    | * -- * -- * |
   *    |/           \|
   *    *             *
  */
  std::vector<graphs::UnitEdge> edges = {
      {1, 2}, {1, 3}, {2, 3}, {1, 4}, {4, 5}, {5, 6}, {5, 7}, {6, 7},
  };
  graphs::UnitGraph graph(7);
  for (const auto& e : edges) {
    graph.AddEdge(e);
    graph.AddEdge({e.to, e.from});
  }
  graphs::Router router(graph);
  ASSERT_EQ(1, router.GetBridges());
}