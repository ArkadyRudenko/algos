#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

// find topology sort

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
        colors_(graph.GetVertexCount() + 1) {
    path_.assign(graph.GetVertexCount() + 1, 0);
  }

  std::vector<VertexId> FindLoop() {
    for (size_t i = 1; i < graph_.GetVertexCount() + 1; ++i) {
      if (DfsImpl(i)) {
        break;
      }
    }
    if (start_ == 0) {
      return {};
    }
    std::vector<VertexId> loop;
    loop.push_back(start_);
    for (int v = end_; v != start_; v = path_[v]) {
      loop.push_back(v);
    }
    std::reverse(loop.begin(), loop.end());
    return loop;
  }

  bool DfsImpl(const VertexId vertex_id) {
    colors_[vertex_id] = Color::Grow;
    for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
      const auto& edge = graph_.GetEdge(edg_id);
      const auto to = edge.to;
      if (colors_[to] == Color::White) {
        path_[to] = vertex_id;
        if (DfsImpl(to)) {
          return true;
        }
      } else if (colors_[to] == Color::Grow) {
        end_ = vertex_id;
        start_ = to;
        return true;
      }
    }
    colors_[vertex_id] = Color::Black;
    return false;
  }

  std::vector<VertexId> FindTopSort() {
    if (!FindLoop().empty()) {
      return {};
    }
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

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<Color> colors_;
  std::vector<VertexId> sort_;
  std::vector<VertexId> path_;
  VertexId start_{0};
  VertexId end_{0};
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
  if (auto sort = router.FindTopSort(); !sort.empty()) {
    for (const auto ver : sort) {
      std::cout << ver << ' ';
    }
  } else {
    std::cout << "-1";
  }
}
