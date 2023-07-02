#include <gtest/gtest.h>

#include <algos/algos.hpp>
#include <algos/graphs/Router.hpp>

TEST(MinLimos, JustWorks) {
  std::vector<int64_t> vec = {1,2,7,9};
  auto res = 2;
  EXPECT_EQ(res, 2);
}

TEST(Graphs, Turtle) {
  std::vector<algos::graph::Edge<size_t>> edges = {
      {1, 2, 1},
      {1, 4, 3},
      {2, 4, 1},
      {2, 3, 3},
      {4, 5, 1},
      {3, 6, 5},
      {5, 6, 1},
//      {7, 8, 1},
      {4, 3, 1},
      {6, 7, 1},
      {7, 8, 5},
      {2, 8, 1},
      {7, 1, 1},
      {1, 8, 1},
  };
  algos::graph::DirectedWeightedGraph<size_t> graph(8);
  for (const auto& e : edges) {
    graph.AddEdge(e);
  }
  algos::graph::Router router(graph);
  std::cout << router.Bfs_1_k(2, 7, 5) << std::endl;
  std::cout << router.Bfs_1_k(2, 8, 5) << std::endl;
  std::cout << router.Bfs_1_k(7, 8, 5) << std::endl;
}

TEST(Graphs, IsLoop) {
  std::vector<algos::graph::UnitEdge> edges = {
      {1, 4},
      {1, 8},

      {2, 4},
      {2, 3},
      {2, 8},

      {3, 6},

      {4, 5},
      {4, 3},

      {5, 6},

      {6, 7},

      {7, 8},
      {7, 1},
//      {8, 8},
  };
  algos::graph::UnitGraph graph(8);
  for (const auto& e : edges) {
    graph.AddEdge(e);
  }
  algos::graph::Router router(graph);
  if (auto loop = router.FindLoop(); !loop.empty()) {
    std::cout << "YES" << std::endl;
    for (const auto ver : loop) {
      std::cout << ver << ' ';
    }
  } else {
    std::cout << "NO";
  }
}

TEST(Graphs, TopSort) {
  std::vector<algos::graph::UnitEdge> edges = {
//      {1, 2},
//      {1, 4},
      {2, 4},
      {2, 3},
      {4, 5},
      {3, 6},
      {5, 6},
      {4, 3},
      {6, 7},
      {7, 8},
      {2, 8},
      {7, 1},
//      {1, 7},

  };
  algos::graph::UnitGraph graph(8);
  for (const auto& e : edges) {
    graph.AddEdge(e);
  }
//  algos::graph::Router router(graph);
//  const auto& sort = router.FindTopSort();
//  for (const auto vertex_id : sort) {
//    std::cout << vertex_id << ' ';
//  }
}