#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

// short path on {1...k} - weight graph

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

struct ShortestPathInfo {
  int path;
  std::vector<size_t> vertexes;
};

template <typename Weight>
class Router {
 private:
  using Graph = DirectedWeightedGraph<Weight>;

  struct Used {
    bool is_used_{false};
  };

  struct Dist {
    int dist{-1};
  };

 public:
  explicit Router(const Graph& graph)
      : graph_(graph),
        dists_(graph.GetVertexCount() + 1),
        vertexes_used_(graph.GetVertexCount() + 1) {}

  int Bfs_1_k(size_t from, size_t to, size_t k) {
    const size_t vertexes_count = graph_.GetVertexCount();
    if (from > vertexes_count or to > vertexes_count) {
      return -1;
    } else if (from == to) {
      return 0;
    }
    const size_t queues_count = (vertexes_count - 1) * k;
    std::vector<std::queue<size_t>> at_dist(k + 1);
    at_dist[1].push(from);
    dists_[from].dist = 0;
    for (size_t i = 1; i < queues_count; ++i) {
      auto& queue = at_dist[i % k];
      while (!queue.empty()) {
        auto cur = queue.front();
        queue.pop();
        for (auto edg_id : graph_.GetIncidentEdges(cur)) {
          const auto& cur_edge = graph_.GetEdge(edg_id);
          const auto cur_to = cur_edge.to;
          if (dists_[cur_to].dist == -1) {
            dists_[cur_to].dist = dists_[cur].dist + cur_edge.weight;
            const auto index = dists_[cur_to].dist % k;
            at_dist[index].push(cur_to);
          } else if (dists_[cur_to].dist > dists_[cur].dist + cur_edge.weight) {
            dists_[cur_to].dist = dists_[cur].dist + cur_edge.weight;
            const auto index = dists_[cur_to].dist % k;
            at_dist[index].push(cur_to);
          }
        }
      }
    }
    auto res = dists_[to].dist;
    dists_ = std::vector<Dist>(graph_.GetVertexCount() + 1);
    return res;
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<Dist> dists_;
};

}  // namespace graphs

int main() {
  std::ios::sync_with_stdio(0);
  std::cin.tie(0);
  size_t vertexes = 0, edges = 0;
  std::cin >> vertexes >> edges;
  graphs::DirectedWeightedGraph<size_t> graph(vertexes);
  size_t from = 0, to = 0;
  std::cin >> from >> to;
  size_t k = 0;
  for (size_t i = 0; i < edges; ++i) {
    size_t begin = 0, end = 0, weight = 0;
    std::cin >> begin >> end >> weight;
    if (weight > k) {
      k = weight;
    }
    graph.AddEdge({begin, end, weight});
  }
  graphs::Router router(graph);
  auto length = router.Bfs_1_k(from, to, k);
  std::cout << length << std::endl;
}
