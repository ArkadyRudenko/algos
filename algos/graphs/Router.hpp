#include <algos/graphs/Graph.hpp>

namespace algos::graph {

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

  ShortestPathInfo MinNotWeightPathFromTo(size_t from, size_t to) {
    const size_t vertexes_count = graph_.GetVertexCount();
    if (from > vertexes_count or to > vertexes_count) {
      return {-1, {}};
    } else if (from == to) {
      return {0, {from}};
    }
    std::queue<size_t> bfs_q;
    std::vector<size_t> path(vertexes_count + 1);
    path[from] = 0;
    dists_[from].dist = 0;
    bfs_q.push(from);
    while (!bfs_q.empty()) {
      size_t cur = bfs_q.front();
      bfs_q.pop();
      for (auto edg_id : graph_.GetIncidentEdges(cur)) {
        auto cur_to = graph_.GetEdge(edg_id).to;
        if (dists_[cur_to].dist == -1) {
          dists_[cur_to].dist = dists_[cur].dist + 1;
          bfs_q.push(cur_to);
          path[cur_to] = cur;
        }
      }
    }
    std::vector<size_t> real_path;
    for (size_t ver_id = to; ver_id != 0; ver_id = path[ver_id]) {
      real_path.push_back(ver_id);
    }
    std::reverse(real_path.begin(), real_path.end());
    return {dists_[to].dist, std::move(real_path)};
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<Dist> dists_;
};

}  // namespace algos::graph
