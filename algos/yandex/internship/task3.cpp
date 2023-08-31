#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Rest {
  std::string column;
  char q;
  int val;
};

int main() {

  int n{0}, m{0}, q{0};
  std::cin >> n >> m >> q;
  std::unordered_map<std::string, int> columns;
  std::vector<int> lines_sum;
  std::unordered_map<std::string, std::pair<int, int>> mini_max;
  std::unordered_map<int, std::string> col_by_index;

  lines_sum.reserve(n);
  columns.reserve(m);
  col_by_index.reserve(m);
  for (int i = 0; i < m; i++) {
    std::string str;
    std::cin >> str;
    columns[str] = i;
    col_by_index[i] = std::move(str);
  }
  std::vector<std::vector<int>> values;
  values.reserve(n);
  for (int i = 0; i < n; i++) {
    values.emplace_back();
    values[i].reserve(m);
    int sum = 0;
    for (int j = 0; j < m; j++) {
      int val;
      std::cin >> val;
      values[i].push_back(val);
      sum += val;
      std::string& column = col_by_index[j];
      if (!mini_max.contains(column)) {
        mini_max[column] = {-1000000001, 1000000001};
      }
      mini_max[column].first = std::max(mini_max[column].first, val);
      mini_max[column].second = std::min(mini_max[column].second, val);
    }
    lines_sum.push_back(sum);
  }
  int res{0};
  std::vector<Rest> rests;
  rests.reserve(q);
  for (int i = 0; i < q; ++i) {
    std::string column;
    char sign;
    int val;
    std::cin >> column >> sign >> val;
    if (sign == '<') {
      if (mini_max[column].second > val) {
        std::cout << 0;
        return 0;
      }
    } else if (sign == '>') {
      if (mini_max[column].first < val) {
        std::cout << 0;
        return 0;
      }
    }
    rests.emplace_back(Rest{column, sign, val});
  }
  std::unordered_set<int> bad_lines;
  // ----- CALCULATE -----
  for (const auto& rest : rests) {
    int index = columns[rest.column];
    for (int str = 0; str < n; ++str) {
      bool bad = false;
      if (rest.q == '<') {
        if (values[str][index] >= rest.val) {
          bad = true;
        }
      } else if (rest.q == '>') {
        if (values[str][index] <= rest.val) {
          bad = true;
        }
      }
      if (bad) {
        bad_lines.insert(str);
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    if (!bad_lines.contains(i)) {
      res += lines_sum[i];
    }
  }
  std::cout << res;
}