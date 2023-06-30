#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using it = std::vector<int>::iterator;

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
  while (true) {
    auto chunks = SplitByFive(spine);
    for (auto& chunk : chunks) {
      BubbleSort(chunk);
    }
    //    for (auto& chunk : chunks) {
    //      std::cout << "chunk: ";
    //      for (auto i : chunk) {
    //        std::cout << i << ' ';
    //      }
    //      std::cout << std::endl;
    //    }
    std::vector<int> medians;
    medians.reserve(chunks.size());
    for (auto& chunk : chunks) {
      if (chunk.size() == 5) {
        medians.push_back(chunk[2]);
      } else {
        medians.push_back(chunk[chunk.size() / 2]);
      }
    }
    if (medians.size() <= 5) {
      return medians[medians.size() / 2];
    }
    spine = std::move(medians);  // TODO
  }
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
    std::iter_swap(left, k_it);
  }
  return (int)(left - begin);
}

int QuickSelectImpl(it begin, it end, int k, bool use_random = false) {
  int pivot = 0;
  if (use_random) {
    pivot = (int)((end - begin) / 2);
  } else {
    pivot = MedianOfMedians(begin, end);
  }
  std::cout << "pivot = " + std::to_string(pivot) << std::endl;
  std::cout << "Array before partition = ";
  auto b = begin;
  while (b != end) {
    std::cout << *b << ' ';
    ++b;
  }
  std::cout << std::endl;
  int i = Partition(begin, end, pivot);
  std::cout << "Array after partition = ";
  b = begin;
  while (b != end) {
    std::cout << *b << ' ';
    ++b;
  }
  std::cout << "\ni = " + std::to_string(i) << std::endl;
  std::cout << "k = " + std::to_string(k) << std::endl;
  if (i > k) {
    return QuickSelectImpl(begin, begin + i, k);
  } else if (i == k) {
    return *(begin + i);
  } else {
    return QuickSelectImpl(begin + i, end, k - i - 1);
  }
}

int QuickSelect(std::vector<int>& arr, int k) {
  return QuickSelectImpl(arr.begin(), arr.end(), k - 1);
}

void QuickSortImpl(it begin, it end) {
  if (std::next(begin) == end) {
    return;
  }
  int pivot = MedianOfMedians(begin, end);
  int i = Partition(begin, end, pivot);
  QuickSortImpl(begin, begin + i);
  QuickSortImpl(begin + i, end);
}

void QuickSort(std::vector<int>& arr) {
  QuickSortImpl(arr.begin(), arr.end());
}

int main() {
  // , k = 0, a0 = 0, a1 = 0
  std::srand(std::time(nullptr));
  int n = 0;
  std::cin >> n;
  std::vector<int> arr;
  for (size_t i = 0; i < n; ++i) {
    arr.push_back(std::rand() % 100);
  }
  //  GenerateArray(arr, k, a0, a1);
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << ' ';
  }
  std::cout << std::endl;
//  QuickSort(arr);
    int k = 0;
    std::cin >> k;
    k = QuickSelect(arr, k);
    std::cout << "QuickSelect = " << k << std::endl;
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << ' ';
  }
  return 0;
}