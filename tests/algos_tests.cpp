#include <gtest/gtest.h>

#include <algos/lib.hpp>

TEST(BinSearch, JustWorks) {
  size_t size_of_array = 0;
  std::cin >> size_of_array;
  int* array = new int[size_of_array];
  for (size_t arr_id = 0; arr_id < size_of_array; ++arr_id) {
    std::cin >> array[arr_id];
  }
  size_t count_of_calling = 0;
  std::cin >> count_of_calling;
  for (size_t call_id = 0; call_id < count_of_calling; ++call_id) {
    size_t begin = 0;
    size_t end = 0;
    size_t target = 0;
    std::cin >> begin >> end >> target;
    if (BinarySearch(&array[begin], &array[end], target)) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
  delete[] array;
  EXPECT_EQ(1, 1);
}