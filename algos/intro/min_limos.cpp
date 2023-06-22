#include <algorithm>
#include <iostream>

bool CanCover(const int64_t* arr, size_t size, int64_t piece, int64_t len) {
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
  int64_t right = std::abs(arr[size - 1] - arr[0]);
  int64_t left = 0;
  while (right - left > 1) {
    int64_t mid = (left + right) / 2;
    if (CanCover(arr, size, piece, mid)) {
      right = mid;
    } else {
      left = mid;
    }
  }
  return right == 0 ? 1 : right;
}

int main() {
  size_t size = 0;
  std::cin >> size;
  int64_t k = 0;
  std::cin >> k;
  auto* arr = new int64_t[size];
  for (size_t i = 0; i < size; ++i) {
    std::cin >> arr[i];
  }
  std::cout << MinLimos(arr, size, k);
  delete[] arr;
  return 0;
}
