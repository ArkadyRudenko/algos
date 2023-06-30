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
 */