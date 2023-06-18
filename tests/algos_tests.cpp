#include <gtest/gtest.h>

#include <algos/algos.hpp>

TEST(MinLimos, JustWorks) {
  std::vector<int64_t> vec = {1,2,7,9};
  auto res = algos::MinLimos(vec.data(), vec.size(), 2);
  EXPECT_EQ(res, 2);
}