#include <gtest/gtest.h>

#include <algos/algos.hpp>
#include <algos/graphs/Router.hpp>

TEST(MinLimos, JustWorks) {
  std::vector<int64_t> vec = {1,2,7,9};
  auto res = algos::MinLimos(vec.data(), vec.size(), 2);
  EXPECT_EQ(res, 2);
}

TEST(MinLimos, Turtle) {
//  std::vector<algos::graph::UnitEdge> edges = {
//      {1, 2},
//      {1, 3},
//      {2, 5},
//      {8, 6},
//      {5, 9},
//      {5, 1},
//      {6, 1},
//      {6, 5},
//  };
//  algos::graphs::UnitGraph graph(10);
//  for (auto e : edges) {
//    graph.AddEdge(e);
//    graph.AddEdge({e.to, e.from});
//  }
//  graph.BfsImpl(1, 8);
}