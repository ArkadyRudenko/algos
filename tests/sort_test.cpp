#include <iostream>

#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using it = std::vector<int>::iterator;

int QuickSelectImpl(it, it, int, bool);

void GenerateArray(std::vector<int>& arr, int k, int a0, int a1) {
  arr.push_back(a0);
  arr.push_back(a1);
  for (int i = 2; i < arr.size(); ++i) {
    arr.push_back((arr[i - 1] * 123 + arr[i - 2] * 45) % (10000000 + 4321));
  }
}

void BubbleSort(std::vector<int>& arr) {
  for (size_t i = 0; i < arr.size() - 1; ++i) {
    for (size_t j = 0; j < arr.size() - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

std::vector<std::vector<int>> SplitByFive(const std::vector<int>& arr) {
  size_t sz = arr.size() % 5 == 0 ? arr.size() / 5 : arr.size() / 5 + 1;
  std::vector<std::vector<int>> chunks;
  chunks.resize(sz);
  for (size_t i = 0; i < arr.size(); ++i) {
    chunks[i / 5].push_back(arr[i]);
  }
  return chunks;
}

int MedianOfMedians(it begin, it end) {
  std::vector<int> spine(begin, end);
  auto chunks = SplitByFive(spine);
  for (auto& chunk : chunks) {
    BubbleSort(chunk);
  }
  std::vector<int> medians;
  medians.reserve(chunks.size());
  for (const auto& chunk : chunks) {
    medians.push_back(chunk[chunk.size() / 2]);
  }
  return QuickSelectImpl(medians.begin(), medians.end(), medians.size() / 10,
                         true);
}

int Partition(it begin, it end, int k) {
  auto left = begin, right = end - 1, k_it = end;
  while (left < right) {
    while (left != end && *left < k) {
      ++left;
    }
    while (right != begin && *right >= k) {
      if (*right == k) {
        k_it = right;
      }
      --right;
    }
    if (left < right) {
      std::iter_swap(left, right);
      if (*right == k) {
        k_it = right;
      }
      left++;
      right--;
    }
  }
  if (k_it != end) {
    if (*left < *k_it) {
      left++;
    }
    std::iter_swap(left, k_it);
  }
  return (int)(left - begin);
}

int QuickSelectImpl(it begin, it end, int k, bool use_random = false) {
  int pivot = 0;
  if (use_random) {
    pivot = *(begin + (std::rand() % (end - begin)));
  } else {
    pivot = MedianOfMedians(begin, end);
  }
  int i = Partition(begin, end, pivot);
  if (i > k) {
    return QuickSelectImpl(begin, begin + i, k);
  } else if (i == k) {
    return *(begin + i);
  } else {
    return QuickSelectImpl(begin + i + 1, end, k - i - 1);
  }
}

int QuickSelect(std::vector<int>& arr, int k) {
  return QuickSelectImpl(arr.begin(), arr.end(), k);
}

void QuickSortImpl(it begin, it end, std::vector<int>& a) {
  if (std::next(begin) == end or begin == end) {
    return;
  }
  int pivot = MedianOfMedians(begin, end);
//  int pivot = *(begin + (end - begin) / 2);
  int i = Partition(begin, end, pivot);
  QuickSortImpl(begin, begin + i, a);
  QuickSortImpl(begin + i + 1, end, a);
}

void QuickSort(std::vector<int>& arr) {
  QuickSortImpl(arr.begin(), arr.end(), arr);
}

TEST(Sort, isSorted) {
  std::srand(std::time(nullptr));
  for (size_t n = 0; n < 100; ++n) {
    std::vector<int> vec;
    vec.reserve(10000);
    for (size_t i = 0; i < 10000; ++i) {
      vec.push_back(std::rand() % 2000);
    }
//    std::sort(vec.begin(), vec.end());
    QuickSort(vec);
    ASSERT_EQ(true, std::is_sorted(vec.begin(), vec.end()));
  }
}

