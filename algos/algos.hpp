#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#include <algos/graphs/Router.hpp>

namespace algos {
}
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

void DfsNotRecursion() const {
  std::stack<VertexId> vertexes;
  for (auto n{0}; auto& is_used : vertexes_used_) {
    if (n != 0 && !is_used.is_used_) {
      vertexes.push(n);
      while (!vertexes.empty()) {
        auto ver_id = vertexes.top();
        vertexes_used_[ver_id].is_used_ = true;
        vertexes.pop();
        for (auto edg_id : vertexes_[ver_id]) {
          vertexes.push(edges_[edg_id].to);
        }
      }
    }
    n++;
  }
}

 ///////////////////////// LOOP
 std::vector<VertexId> FindLoop() {
 for (auto n{0}; auto& is_used : vertexes_used_) {
   if (n != 0 && !is_used.is_used_) {
     DfsImpl(n);
   }
   n++;
 }
 return real_path_;
}

void DfsImpl(const VertexId vertex_id) {
vertexes_used_[vertex_id].is_used_ = true;
colors_[vertex_id] = Color::Grow;
path_.push_back(vertex_id);
std::vector<Used> to_vertexes;
to_vertexes.reserve(graph_.GetVertexCount() + 1);
for (auto edg_id : graph_.GetIncidentEdges(vertex_id)) {
 const auto& edge = graph_.GetEdge(edg_id);
 if (to_vertexes[edge.to].is_used_) {
   continue;
 }
 if (edge.to == vertex_id) {
   is_done_ = true;
   real_path_.clear();
   real_path_.push_back(vertex_id);
 }
 to_vertexes[edge.to].is_used_ = true;
 if (colors_[edge.to] == Color::White) {
   DfsImpl(edge.to);
 } else if (colors_[edge.to] == Color::Grow) {
   if (!is_done_) {
     is_done_ = true;
     real_path_ = path_;
   }
 }
}
colors_[vertex_id] = Color::Black;
path_.pop_back();
}
 */