#include <algorithm>
#include <iostream>
#include <string>

// Min-stack

class MinStack {
 public:
  MinStack()
      : values_(new size_t[1000]), mines_(new size_t[1000]), capacity_(1000) {}

  void Push(size_t value) {
    if (sz_ == capacity_) {
      auto* new_mines = new size_t[capacity_ * 2];
      auto* new_values = new size_t[capacity_ * 2];
      for (size_t index = 0; index < sz_; ++index) {
        new_mines[index] = mines_[index];
        new_values[index] = values_[index];
      }
      delete[] values_;
      delete[] mines_;
      mines_ = new_mines;
      values_ = new_values;
      capacity_ *= 2;
    }
    values_[sz_] = value;
    if (sz_ != 0) {
      if (value < mines_[sz_ - 1]) {
        mines_[sz_] = value;
      } else {
        mines_[sz_] = mines_[sz_ - 1];
      }
    } else {
      mines_[0] = value;
    }
    ++sz_;
  }

  void Pop() {
    if (sz_ > 0) {
      --sz_;
    }
  }

  [[nodiscard]] size_t Top() const { return values_[sz_ - 1]; }

  [[nodiscard]] size_t Min() const { return mines_[sz_ - 1]; }

  [[nodiscard]] size_t Size() const { return sz_; }

  void Clear() { sz_ = 0; }

  ~MinStack() {
    delete[] values_;
    delete[] mines_;
  }

 private:
  size_t* values_;
  size_t* mines_;
  size_t sz_ = 0;
  size_t capacity_ = 0;
};

void MakeActionWithStack(const std::string& command, MinStack& min_stack) {
  if ("push" == command) {
    size_t value;
    std::cin >> value;
    min_stack.Push(value);
    std::cout << "ok\n";
  } else if ("pop" == command) {
    size_t poped = min_stack.Top();
    min_stack.Pop();
    std::cout << poped << '\n';
  } else if ("back" == command) {
    std::cout << min_stack.Top() << '\n';
  } else if ("min" == command) {
    std::cout << min_stack.Min() << '\n';
  } else if ("size" == command) {
    std::cout << min_stack.Size() << '\n';
  } else if ("clear" == command) {
    min_stack.Clear();
    std::cout << "ok\n";
  }
}

void Process(int commands, MinStack& min_stack) {
  std::string command;
  for (int i = 0; i < commands; ++i) {
    std::cin >> command;
    if (command == "pop" || command == "back" || command == "min") {
      if (min_stack.Size() == 0) {
        std::cout << "error" << '\n';
        continue;
      }
    }
    MakeActionWithStack(command, min_stack);
  }
}

int main() {
  int n_command = 0;
  std::cin >> n_command;
  MinStack min_stack = MinStack();
  Process(n_command, min_stack);
  return 0;
}
