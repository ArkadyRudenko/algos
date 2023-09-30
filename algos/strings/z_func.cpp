#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void BuildZ(const std::string& s, int start) {
  std::vector<int> z;
  z.resize(s.size(), 0);
  z[0] = (int)s.size();
  int l = 0, r = 0;
  for (int i = start; i < s.size() - start + 2; ++i) {
    z[i] = std::max(0, std::min(z[i - l], r - i + 1));
    while (i + z[i] < s.size() && s[i + z[i]] == s[z[i]]) {
      z[i]++;
    }
    if (z[i] == start - 1) {
      std::cout << i - start << '\n';
    }
    if (r < i + z[i]) {
      r = i + z[i];
      l = i;
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false), std::cin.tie(nullptr),
      std::cout.tie(nullptr);
  std::string a, b;
  std::cin >> a >> b;
  BuildZ(b + "#" + a, b.size() + 1);
}