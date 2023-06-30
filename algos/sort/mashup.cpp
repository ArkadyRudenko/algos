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
  int* starts = new int[n];
  int* ends = new int[n];
  int* results = new int[n * 2];
  for (int i = 0; i < n; ++i) {
    int start = 0;
    int end = 0;
    std::cin >> start >> end;
    starts[i] = start;
    ends[i] = end;
  }
  MergeSort(starts, 0, n - 1);
  for (size_t i = 0; i < n; ++i) {
    std::cout << starts[i] << ' ';
  }
  MergeSort(ends, 0, n - 1);
  int start_index = 0, end_index = 0;
  int res_size = 0;
  auto start1 = starts[start_index];
  auto end1 = ends[end_index];
  auto start2 = starts[++start_index];
  auto end2 = ends[++end_index];
  while (true) {
    if (end1 >= start2) {
      end1 = end2;
      if (start_index == n or end_index == n) {
        results[res_size++] = start1;
        results[res_size++] = end1;
        break;
      }
      start2 = starts[start_index++];
      end2 = ends[end_index++];
    } else {
      results[res_size++] = start1;
      results[res_size++] = end1;
      if (start_index == n or end_index == n) {
        results[res_size++] = start2;
        results[res_size++] = end2;
        break;
      }
      start1 = start2;
      end1 = end2;
      start2 = starts[start_index];
      end2 = starts[end_index];
    }
  }
  std::cout << "\n-----starts--------\n";
  for (size_t i = 0; i < n; ++i) {
    std::cout << starts[i] << ' ';
  }
  std::cout << "\n-------ends------\n";
  for (size_t i = 0; i < n; ++i) {
    std::cout << ends[i] << ' ';
  }
  std::cout << "\n-------------\n";
  std::cout << res_size / 2 << std::endl;
  for (size_t i = 0; i < res_size; i += 2) {
    std::cout << results[i] << ' ' << results[i + 1] << std::endl;
  }
  delete[] starts;
  delete[] results;
  delete[] ends;
  return 0;
}

/*
7
8 10
1 3
5 8
2 4
1001 1002
550 558
558 559
 */