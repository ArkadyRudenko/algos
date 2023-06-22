#include <iostream>

void Process(int** arr, const size_t sz, const size_t req) {
  for (size_t i = 0; i < req; ++i) {
    size_t left = 0;
    size_t right = 0;
    std::cin >> left >> right;
    std::cout << std::min(arr[right][sz], arr[1][left]) << std::endl;
  }
}

int main() {
  size_t n = 0;
  std::cin >> n;
  int** arr = new int*[n + 1];
  for (size_t i = 1; i < n + 1; ++i) {
    arr[i] = new int[n + 1];
  }
  for (size_t i = 1; i < n + 1; ++i) {
    std::cin >> arr[i][0];
  }
  for (size_t i = 1; i < n + 1; ++i) {
    for (size_t j = i; j < n + 1; ++j) {
      if (i == j) {
        arr[i][j] = arr[i][0];
      } else {
        arr[i][j] = std::min(arr[j][0], arr[i][j - 1]);
      }
    }
  }
  size_t req = 0;
  std::cin >> req;
  Process(arr, n, req);
  for (size_t i = 1; i < n + 1; ++i) {
    delete[] arr[i];
  }
  delete[] arr;
  return 0;
}
