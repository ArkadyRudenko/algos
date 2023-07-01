#include <cstdlib>
#include <deque>
#include <iostream>
#include <vector>

namespace graph {

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

struct Unit {};

template <typename Weight>
class DirectedWeightedGraph {
 private:
  using IncidenceList = std::vector<EdgeId>;
  using IncidentEdgesRange = Range<typename IncidenceList::const_iterator>;

  struct Used {
    bool is_used_{false};
  };

 public:
  explicit DirectedWeightedGraph(size_t vertex_count = 0)
      : vertexes_(vertex_count) {}

  EdgeId AddEdge(const Edge<Weight>& edge) {
    edges_.push_back(edge);
    const EdgeId id = edges_.size() - 1;
    vertexes_[edge.from].push_back(id);
    return id;
  }

  size_t GetVertexCount() const { return vertexes_.size(); }

  size_t GetEdgeCount() const { return edges_.size(); }

  const Edge<Weight>& GetEdge(EdgeId edge_id) const { return edges_[edge_id]; }

  IncidentEdgesRange GetIncidentEdges(VertexId vertex) const {
    const auto& edges = vertexes_[vertex];
    return {std::begin(edges), std::end(edges)};
  }

  size_t GetMinRoute(size_t begin_id, size_t end_id) const { return 0; }

  // This Dfs only for not directed not weight
  size_t Dfs() const {
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        DfsImpl(n);
      }
      n++;
    }
    return 0;
  }

  size_t DfsImpl(size_t vertex_id) const {
    vertexes_used_[vertex_id].is_used_ = true;
    std::cout << "VertexId = " << vertex_id << std::endl;
    for (auto ver_id : vertexes_[vertex_id]) {
      if (!vertexes_used_[ver_id].is_used_) {
        DfsImpl(ver_id);
      }
    }
    return 0;
  }

 private:
  std::vector<Edge<Weight>> edges_;
  std::vector<IncidenceList> vertexes_;
  mutable std::vector<Used> vertexes_used_;
};

using UnitGraph = DirectedWeightedGraph<Unit>;

}  // namespace graph
