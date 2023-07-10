#include <iostream>

// TODO

class AVLTree {
 public:
  void Add(int data) {
    if (root_ == nullptr) {
      root_ = new Node{data};
    } else {

    }
  }

  ~AVLTree() {}

 private:
  struct Node {
    int data_{0};
    Node* left_{nullptr};
    Node* right_{nullptr};
  };

 private:
  Node* root_{nullptr};
};

int main() {
  return 0;
}