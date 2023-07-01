#include <iostream>

// Correct [{()}] order

namespace hippogriff {

class Stack {
 public:
  Stack() : values_(new char[1000]), capacity_(1000) {}

  void Push(char value) {
    if (sz_ == capacity_) {
      auto* new_values = new char[capacity_ * 2];
      for (size_t index = 0; index < sz_; ++index) {
        new_values[index] = values_[index];
      }
      delete[] values_;
      values_ = new_values;
      capacity_ *= 2;
    }
    values_[sz_] = value;
    ++sz_;
  }

  void Pop() {
    if (sz_ > 0) {
      --sz_;
    }
  }

  [[nodiscard]] size_t Top() const { return values_[sz_ - 1]; }

  [[nodiscard]] size_t Size() const { return sz_; }

  void Clear() { sz_ = 0; }

  ~Stack() { delete[] values_; }

 private:
  char* values_;
  size_t sz_ = 0;
  size_t capacity_ = 0;
};

int main() {
  std::string input;
  std::cin >> input;
  Stack stack;
  for (auto ch : input) {
    if (ch == '{' || ch == '[' || ch == '(') {
      stack.Push(ch);
    } else if (stack.Size() == 0) {
      std::cout << "NO";
      return 0;
    } else if (ch == ')' && stack.Top() == '(') {
      stack.Pop();
    } else if (ch == ']' && stack.Top() == '[') {
      stack.Pop();
    } else if (ch == '}' && stack.Top() == '{') {
      stack.Pop();
    } else {
      std::cout << "NO";
      return 0;
    }
  }
  if (stack.Size() == 0) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
}

}  // namespace hippogriff
