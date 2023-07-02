#pragma once

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

  enum class Color {
    White = 0,
    Grow = 1,
    Black = 2,
  };

 public:
  explicit Router(const Graph& graph)
      : graph_(graph),
        dists_(graph.GetVertexCount() + 1),
        vertexes_used_(graph.GetVertexCount() + 1),
        colors_(graph.GetVertexCount() + 1) {
    path_.assign(graph.GetVertexCount() + 1, 0);
  }

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

  std::vector<VertexId> FindLoop_() {
    std::stack<VertexId> vertexes;
    std::vector<size_t> path(graph_.GetVertexCount() + 1);
    path.assign(graph_.GetVertexCount() + 1, 0);
    size_t start_index = 0;
    size_t end_index = 0;
    bool looped = false;
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        vertexes.push(n);
        while (!vertexes.empty()) {
          auto ver_id = vertexes.top();
          vertexes.pop();
          vertexes_used_[ver_id].is_used_ = true;
          for (auto edg_id : graph_.GetIncidentEdges(ver_id)) {
            const auto& edge = graph_.GetEdge(edg_id);
            const auto to = edge.to;
            if (!vertexes_used_[to].is_used_) {
              path[to] = ver_id;
              vertexes.push(to);
            } else {
              start_index = to;
              end_index = ver_id;
              looped = true;
              break;
            }
          }
          if (looped) {
            break;
          }
        }
        if (looped) {
          break;
        }
      }
      n++;
    }
    std::vector<VertexId> loop;
    if (looped) {
      loop.push_back(start_index);
    }
    for (size_t ver_id = end_index; ver_id != start_index;
         ver_id = path[ver_id]) {
      loop.push_back(ver_id);
    }
    std::reverse(loop.begin(), loop.end());
    return loop;
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
        DfsImpl(edge.to);
      }
    }
    sort_.push_back(vertex_id);
  }

 private:
  const Graph& graph_;
  std::vector<Used> vertexes_used_;
  std::vector<Color> colors_;
  std::vector<Dist> dists_;
  std::vector<VertexId> sort_;
  std::vector<VertexId> path_;
  VertexId start_{0};
  VertexId end_{0};
};

}  // namespace algos::graph
