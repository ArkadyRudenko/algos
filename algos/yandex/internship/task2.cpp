#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  std::unordered_map<int, int> a, b;
  int size_a{0}, size_b{0}, q{0};
  std::cin >> size_a >> size_b >> q;
  for (int i = 0; i < size_a; ++i) {
    int c{0};
    std::cin >> c;
    a[c]++;
  }
  for (int i = 0; i < size_b; ++i) {
    int c{0};
    std::cin >> c;
    b[c]++;
  }

  int res{0};

  for (int i = 0; i < q; ++i) {
    int type;
    char pl;
    int card;
    int was;
    std::cin >> type >> pl >> card;
    was = std::abs(a[card] - b[card]);
    if (type == -1) {
      if (pl == 'A') {
        a[card]--;
      } else if (pl == 'B') {
        b[card]--;
      }
    } else if (type == 1) {
      if (pl == 'A') {
        a[card]++;
      } else if (pl == 'B') {
        b[card]++;
      }
    }
    if (i == 0) {
      for (auto [key, value] : a) {
        if (b.contains(key)) {
          res += std::abs(value - b[key]);
        } else {
          res += value;
        }
      }
      for (auto [key, value] : b) {
        if (!a.contains(key)) {
          res += value;
        }
      }
    } else {
      int uniques = std::abs(a[card] - b[card]);
      if (uniques < was) {
        res--;
      } else {
        res++;
      }
    }
    std::cout << res << ' ';
  }
}