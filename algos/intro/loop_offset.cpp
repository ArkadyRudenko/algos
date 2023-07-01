#include <iostream>

// find offset if sorted array
// [1, 2, 3, 4, 5, 6] -> [5, 6, 1, 2, 3, 4]
// offset = 2

size_t CalculateLoopOffset(const int* begin, size_t size, size_t offset) {
  if (begin[0] < begin[size - 1] || size == 1) {
    return offset;
  }
  if (size == 2 && begin[0] > begin[1]) {
    return offset + 1;
  }
  size_t middle_index = size / 2;
  if (begin[0] > begin[middle_index]) {
    return CalculateLoopOffset(begin, middle_index + 1, offset);
  }
  size_t new_size = size % 2 == 0 ? middle_index : middle_index + 1;
  return CalculateLoopOffset(begin + middle_index, new_size,
                             offset + middle_index);
}

int main() {
  size_t size_of_array = 0;
  std::cin >> size_of_array;
  int* array = new int[size_of_array];
  for (size_t arr_id = 0; arr_id < size_of_array; ++arr_id) {
    std::cin >> array[arr_id];
  }
  std::cout << CalculateLoopOffset(array, size_of_array, 0);
  delete[] array;
  return 0;
}
