#include <iostream>

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

}  // namespace algos
