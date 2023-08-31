#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  int n{0};
  std::cin >> n;
  std::vector<std::vector<int>> arrs;
  arrs.reserve(n);
  for (int i = 0; i < n; ++i) {
    arrs.emplace_back();
    int m{0};
    std::cin >> m;
    arrs[i].reserve(m);
    for (int j = 0; j < m; ++j) {
      int val{0};
      std::cin >> val;
      arrs[i].push_back(val);
    }
  }
  int res{0};
  for (int i = 0; i < n - 1; ++i) {
    for (int j = i + 1; j < n; ++j) {
      auto it1 = arrs[i].begin();
      auto it2 = arrs[j].begin();
      int local_res{0};
      while (it1 != arrs[i].end() && it2 != arrs[i].end()) {
        if (*it1 == *it2) {
          local_res++;
        } else {
          break;
        }
        it1++;it2++;
      }
      res += local_res;
    }

  }
  std::cout << res;
}