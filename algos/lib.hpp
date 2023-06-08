#include <iostream>

// 1.1
bool BinarySearch(const int* begin, const int* end, int target) {
  if (begin == end) {
    return false;
  }
  size_t middle_index = (end - begin) / 2;
  if (begin[middle_index] == target) {
    return true;
  }
  if (begin[middle_index] > target) {
    return BinarySearch(begin, begin + middle_index, target);
  }
  return BinarySearch(begin + middle_index + 1, end, target);
}