#include <iostream>

int main() {
  std::string s = "6 17 2 4 0 10 17 17 9 1 7 2 7 4 11 7 14 6 13 3 11 12 19 22 22 21 22 23 21 24 24 23 26";
  int i = 0;
  for (auto c : s) {
    if (c == ' ') {
      ++i;
    }
  }
  std::cout << i + 1 << std::endl;
}