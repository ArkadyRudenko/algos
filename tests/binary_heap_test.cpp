#include <gtest/gtest.h>
#include <iostream>
#include <vector>

class BinaryHeap {
 public:
  void Add(int data) {
    const auto old_size = tree_.size();
    tree_.push_back(data);
    SiftUp(old_size);
  }

  [[nodiscard]] int GetMin() const { return tree_.front(); }

  void ExtractMin() {
    if (tree_.size() == 1) {
      tree_.pop_back();
    } else if (!tree_.empty()) {
      std::swap(tree_.front(), tree_.back());
      tree_.pop_back();
      SiftDown();
    }
  }

  void Print() const {
    for (const auto val : tree_) {
      std::cout << val << ' ';
    }
  }

 private:
  void SiftUp(size_t pos) {
    if (tree_.size() != 1) {
      while (pos != 0 && ParentOf(pos) > tree_[pos]) {
        std::swap(tree_[pos], tree_[IndexOfParent(pos)]);
        pos = IndexOfParent(pos);
      }
    }
  }

  void SiftDown() {
    size_t cur = 0;
    // TODO: easier
    while (LeftIsLess(cur) or RightIsLess(cur)) {
      if (LeftIsLess(cur) && RightIsLess(cur)) {
        if (LeftOf(cur) < RightOf(cur)) {
          std::swap(tree_[cur], tree_[IndexOfLeftChild(cur)]);
          cur = IndexOfLeftChild(cur);
        } else {
          std::swap(tree_[cur], tree_[IndexOfRightChild(cur)]);
          cur = IndexOfRightChild(cur);
        }
      } else {
        if (tree_[cur] > LeftOf(cur)) {
          std::swap(tree_[cur], tree_[IndexOfLeftChild(cur)]);
          cur = IndexOfLeftChild(cur);
        } else if (tree_[cur] > RightOf(cur)) {
          std::swap(tree_[cur], tree_[IndexOfRightChild(cur)]);
          cur = IndexOfRightChild(cur);
        }
      }
    }
  }

  bool LeftIsLess(size_t parent) {
    return (IndexOfLeftChild(parent) < tree_.size() &&
            tree_[parent] > LeftOf(parent));
  }

  bool RightIsLess(size_t parent) {
    return (IndexOfRightChild(parent) < tree_.size() &&
            tree_[parent] > RightOf(parent));
  }

  [[nodiscard]] int ParentOf(size_t i) const { return tree_[IndexOfParent(i)]; }

  [[nodiscard]] int LeftOf(size_t i) const {
    return tree_[IndexOfLeftChild(i)];
  }

  [[nodiscard]] int RightOf(size_t i) const {
    return tree_[IndexOfRightChild(i)];
  }

  [[nodiscard]] size_t IndexOfLeftChild(size_t i) const { return 2 * i + 1; }

  [[nodiscard]] size_t IndexOfRightChild(size_t i) const { return 2 * i + 2; }

  [[nodiscard]] size_t IndexOfParent(size_t i) const { return (i - 1) / 2; }

 private:
  std::vector<int> tree_;
};

TEST(BinaryHeap, test1) {
  BinaryHeap heap;
  heap.Add(5);
  heap.Add(2);
  ASSERT_EQ(2, heap.GetMin());
  heap.Add(1);
  heap.Add(1);
  heap.Add(1);
  heap.Add(1);
  ASSERT_EQ(1, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(1, heap.GetMin());
  heap.ExtractMin();
  heap.ExtractMin();
  heap.ExtractMin();
  ASSERT_EQ(2, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(5, heap.GetMin());
}

TEST(BinaryHeap, test2) {
  BinaryHeap heap;
  heap.Add(5);
  heap.Add(4);
  heap.Add(3);
  heap.Add(2);
  heap.Add(1);
  ASSERT_EQ(1, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(2, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(3, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(4, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(5, heap.GetMin());
  heap.ExtractMin();
  heap.ExtractMin();
}

TEST(BinaryHeap, test3) {
  BinaryHeap heap;
  heap.Add(5);
  heap.Add(4);
  heap.Add(3);
  heap.Add(2);
  heap.Add(1);
  ASSERT_EQ(1, heap.GetMin());
  heap.Add(-1);
  heap.Add(6);
  heap.Add(7);
  ASSERT_EQ(-1, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(1, heap.GetMin());
  heap.ExtractMin();
  heap.ExtractMin();
  heap.ExtractMin();
  ASSERT_EQ(4, heap.GetMin());
}

TEST(BinaryHeap, stress) {
  BinaryHeap heap;
  std::srand(std::time(nullptr));
  for (size_t i = 0; i < 10000; ++i) {
    heap.Add(std::rand() % 1000);
  }
  const auto min = heap.GetMin();
  heap.Add(-1);
  ASSERT_EQ(-1, heap.GetMin());
  heap.ExtractMin();
  ASSERT_EQ(min, heap.GetMin());
  for (size_t i = 0; i < 10000; ++i) {
    heap.Add(std::rand() % 1000);
  }
  std::vector<int> vec;
  for (size_t i = 0; i < 20000; ++i) {
    vec.push_back(heap.GetMin());
    heap.ExtractMin();
  }
  ASSERT_EQ(true, std::is_sorted(vec.begin(), vec.end()));
}