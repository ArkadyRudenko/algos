#include <iostream>
#include <queue>
#include <stack>
#include <vector>

namespace algos {

bool CoverageCheck(const int64_t* arr, size_t size, int64_t piece,
                   int64_t len) {
  if (len == 0) {
    return false;
  }
  int64_t cur_pos = arr[0];
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] > cur_pos || i == 0) {
      if (piece == 0) {
        return false;
      }
      cur_pos = arr[i] + len;
      piece--;
    }
  }
  return true;
}

size_t MinLimos(int64_t* arr, size_t size, int64_t piece) {
  std::sort(arr, arr + size);
  int64_t len = std::abs(arr[size - 1] - arr[0]);
  int64_t min_len = len / (int64_t)piece;
  if (min_len == 0) {
    min_len = 1;
  }
  int64_t next_plus = min_len / 2;
  while (true) {
    bool too_short = CoverageCheck(arr, size, piece, min_len - 1);
    bool too_enough = CoverageCheck(arr, size, piece, min_len);
    if (!too_short && too_enough) {
      return min_len;
    }
    if (too_short) {
      min_len -= next_plus;
    } else {
      min_len += next_plus;
    }
    next_plus /= 2;
    if (next_plus == 0) {
      next_plus = 1;
    }
  }
}

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

struct Unit {};

using UnitEdge = Edge<Unit>;

template <typename Weight>
class DirectedWeightedGraph {
 private:
  using IncidenceList = std::vector<EdgeId>;
  using IncidentEdgesRange = Range<typename IncidenceList::const_iterator>;

  struct Used {
    bool is_used_{false};
  };

  struct Dist {
    int dist{-1};
  };

 public:
  explicit DirectedWeightedGraph(size_t vertex_count = 0)
      : vertexes_(vertex_count + 1),
        vertexes_used_(vertex_count + 1),
        dists_(vertex_count + 1) {}

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
    vertexes_used_ = std::vector<Used>(vertexes_.size());
    return 0;
  }

  size_t DfsImpl(size_t vertex_id) const {
    vertexes_used_[vertex_id].is_used_ = true;
    std::cout << "VertexId = " << vertex_id << std::endl;
    for (auto edg_id : vertexes_[vertex_id]) {
      auto ver_id = edges_[edg_id].to;
      if (!vertexes_used_[ver_id].is_used_) {
        DfsImpl(ver_id);
      }
    }
    return 0;
  }

  void DfsNotRecursion() const {
    std::stack<VertexId> vertexes;
    for (auto n{0}; auto& is_used : vertexes_used_) {
      if (n != 0 && !is_used.is_used_) {
        vertexes.push(n);
        while (!vertexes.empty()) {
          auto ver_id = vertexes.top();
          vertexes_used_[ver_id].is_used_ = true;
          std::cout << "VertexId = " << ver_id << std::endl;
          vertexes.pop();
          for (auto edg_id : vertexes_[ver_id]) {
            vertexes.push(edges_[edg_id].to);
          }
        }
      }
      n++;
    }
    vertexes_used_ = std::vector<Used>(vertexes_.size());
  }

  void Bfs() const {
    for (size_t ver_id{0}; auto& dist : dists_) {
      if (ver_id != 0 && dist.dist == -1) {
        BfsImpl(ver_id);
      }
      ver_id++;
    }
    dists_ = std::vector<Dist>(dists_.size());
  }

  void BfsImpl(size_t v, size_t t) const {
    if (v > vertexes_.size() - 1 || t > vertexes_.size() - 1) {
      std::cout << "-1" << std::endl;
    }
    if (v == t) {
      std::cout << 0 << std::endl;
      std::cout << v << std::endl;
      return;
    }
    std::queue<size_t> bfs_q;
    std::vector<size_t> path(vertexes_.size() + 1);
    path[v] = 0;
    dists_[v].dist = 0;
    bfs_q.push(v);
    while (!bfs_q.empty()) {
      size_t cur = bfs_q.front();
      bfs_q.pop();
      for (auto edg_id : vertexes_[cur]) {
        auto to = edges_[edg_id].to;
        if (dists_[to].dist == -1) {
          dists_[to].dist = dists_[cur].dist + 1;
          bfs_q.push(to);
          path[to] = cur;
        }
      }
    }
    std::cout << dists_[t].dist << std::endl;
    if (dists_[t].dist != -1) {
      std::vector<size_t> real_path;
      for (size_t ver_id = t; ver_id != 0; ver_id = path[ver_id]) {
        real_path.push_back(ver_id);
      }
      std::reverse(real_path.begin(), real_path.end());
      for (auto p : real_path) {
        std::cout << p << " ";
      }
    }
  }

 private:
  std::vector<Edge<Weight>> edges_;
  std::vector<IncidenceList> vertexes_;
  mutable std::vector<Used> vertexes_used_;
  mutable std::vector<Dist> dists_;
};

using UnitGraph = DirectedWeightedGraph<Unit>;

}  // namespace graphs

}  // namespace algos

/*
#include <iostream>

void Merge(int* arr, int l, int m, int r) {
  auto sz1 = m - l + 1;
  auto sz2 = r - m;
  int* arr1 = new int[sz1];
  int* arr2 = new int[sz2];
  for (size_t i = 0; i < sz1; ++i) {
    arr1[i] = arr[l + i];
  }
  for (size_t i = 0; i < sz2; ++i) {
    arr2[i] = arr[m + i + 1];
  }
  int i = 0, j = 0, index = l;
  while (i < sz1 && j < sz2) {
    if (arr1[i] < arr2[j]) {
      arr[index++] = arr1[i];
      ++i;
    } else {
      arr[index++] = arr2[j];
      ++j;
    }
  }
  while (i < sz1) {
    arr[index++] = arr1[i];
    ++i;
  }
  while (j < sz2) {
    arr[index++] = arr2[j];
    ++j;
  }
  delete[] arr1;
  delete[] arr2;
}

void MergeSort(int* arr, int l, int r) {
  if (l < r) {
    int m = l + (r - l) / 2;
    MergeSort(arr, l, m);
    MergeSort(arr, m + 1, r);
    Merge(arr, l, m, r);
  }
}

int main() {
  int n = 0;
  std::cin >> n;
  int* arr = new int[n];
  for (int i = 0; i < n; ++i) {
    int val = 0;
    std::cin >> val;
    arr[i] = val;
  }
  MergeSort(arr, 0, n - 1);
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << ' ';
  }

  delete[] arr;
  return 0;
}
 */