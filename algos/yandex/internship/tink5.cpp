#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

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

  bool operator<(const Edge<Weight> rhs) const {
    return this->weight < rhs.weight;
  }
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
class ConnectedChecker {
  using Graph = DirectedWeightedGraph<Weight>;

  struct Used {
    bool is_used_{false};
  };

 public:
  explicit ConnectedChecker(const Graph& graph)
      : graph_(graph), vertexes_used_(graph.GetVertexCount() + 1) {}

  bool Check() {
    DfsImpl(graph_.GetEdge(0).from);
    return used_ == graph_.GetVertexCount();
  }

  void DfsImpl(VertexId vertex_id) {
    vertexes_used_[vertex_id].is_used_ = true;
    used_++;
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      if (!vertexes_used_[edge.to].is_used_) {
        DfsImpl(edge.to);
      }
    }
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  size_t used_{0};
};

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

  //  using Edge = Edge<Weight>;

 public:
  explicit Router(const Graph& graph)
      : graph_(graph),
        vertexes_used_(graph.GetVertexCount() + 1),
        ret(graph.GetVertexCount() + 1),
        times_(graph.GetVertexCount() + 1) {}

  std::vector<EdgeId> GetBridges() {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        components_.emplace_back();
        local_max = -1;
        DfsImpl(n);
        components_.back().vertex_count = local_vertexes_count_;
        local_vertexes_count_ = 0;
      }
      n++;
    }
    for (auto bridge : bridges) {
      bridges_set_.insert(bridge);
    }
    return bridges;
  }

  void DfsImpl(VertexId vertex_id,
               std::optional<VertexId> parent_id = std::nullopt) {
    local_vertexes_count_++;
    vertexes_used_[vertex_id].is_used_ = true;
    times_[vertex_id] = ret[vertex_id] = time_++;
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      components_.back().edges.insert(edge);
      if (edge.weight > local_max) {
        local_max = edge.weight;
        components_.back().max = local_max;
      }
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

  int GetMaxX() {
    std::vector<Weight> result;
    result.reserve(components_.size());
    for (auto& component : components_) {
      Weight max{0};
      size_t skip{1};
      for (auto& edge : component.edges) {
        if (bridges_set_.find(edge.real_id) != bridges_set_.end()) {
          max = edge.weight - 1;
          break;
        }
        Graph new_graph(component.vertex_count);
        auto local_skip = skip;
        for (auto new_edge : component.edges) {
          if (local_skip > 0) {
            local_skip--;
            continue;
          }
          new_edge.from = new_edge.from % component.vertex_count + 1;
          new_edge.to = new_edge.to % component.vertex_count + 1;
          new_graph.AddEdge(new_edge);
          if (new_edge.to != new_edge.from) {
            new_graph.AddEdge({new_edge.to, new_edge.from, new_edge.real_id, new_edge.weight});
          }
        }
        if (new_graph.GetEdgeCount() == 0) {
          max = edge.weight-1;
          break;
        }
        ConnectedChecker<Weight> checker(new_graph);
        if (!checker.Check()) {
          max = edge.weight - 1;
          break;
        }
        max = edge.weight;

        skip++;
      }
      result.push_back(max);
    }

    return *std::min_element(result.begin(), result.end());
  }

  struct Component {
    std::set<Edge<Weight>> edges;
    Weight max{-1};
    size_t vertex_count{0};
  };

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<size_t> ret;
  std::vector<size_t> times_;
  size_t time_{0};
  std::vector<EdgeId> bridges;
  std::vector<Component> components_;
  Weight local_max{-1};
  size_t local_vertexes_count_{};
  std::unordered_set<EdgeId> bridges_set_;
};

}  // namespace graphs

int main() {
  size_t vertexes = 0, edges = 0;
  std::cin >> vertexes >> edges;
  graphs::DirectedWeightedGraph<int64_t> graph(vertexes);
  for (size_t i = 0; i < edges; ++i) {
    size_t begin = 0, end = 0;
    int64_t weight = 0;
    std::cin >> begin >> end >> weight;
    if (begin != end) {
      graph.AddEdge({begin, end, i, weight});
      graph.AddEdge({end, begin, i + 1, weight});
    } else {
      graph.AddEdge({begin, end, i, weight});
    }
  }
  graphs::Router router(graph);
  auto bridges = router.GetBridges();
  std::cout << router.GetMaxX();
}
