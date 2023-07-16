#include <iostream>
#include <vector>

int Task(const std::vector<size_t>& vec) {
  const auto value_1 = vec.front();
  auto it = vec.begin();
  while (it != vec.end() && value_1 == *it) {
    it++;
  }
  if (it == vec.end()) {
    return 0;
  }
  const auto value_2 = *it;
  if (value_2 < value_1) {
    return -1;
  }
  while (it != vec.end() && value_2 == *it) {
    it++;
  }
  if (it == vec.end()) {
    return static_cast<int>(vec.back() - vec.front());
  }
  return -1;
}

int main() {
  size_t sz = 0;
  std::cin >> sz;
  std::vector<size_t> vec;
  vec.reserve(sz);
  for (size_t i = 0; i < sz; ++i) {
    size_t value = 0;
    std::cin >> value;
    vec.push_back(value);
  }
  std::cout << Task(vec);
  return 0;
}