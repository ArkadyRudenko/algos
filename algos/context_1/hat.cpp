#include <algorithm>
#include <iostream>
#include <string>

namespace hat {

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
      mines_[sz_] = std::min(value, mines_[sz_ - 1]);
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

  [[nodiscard]] bool IsEmpty() const { return Size() == 0; }

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

class MinQueue {
 public:
  void Push(size_t value) { s1_.Push(value); }

  size_t Front() {
    if (s2_.IsEmpty()) {
      MoveValues();
    }
    return s2_.Top();
  }

  void Pop() {
    if (s2_.IsEmpty()) {
      MoveValues();
    }
    s2_.Pop();
  }

  [[nodiscard]] size_t Min() const {
    if (s1_.IsEmpty() && !s2_.IsEmpty()) {
      return s2_.Min();
    }
    if (!s1_.IsEmpty() && s2_.IsEmpty()) {
      return s1_.Min();
    }
    return std::min(s1_.Min(), s2_.Min());
  }

  size_t Size() { return s2_.Size() + s1_.Size(); }

  void Clear() {
    s1_.Clear();
    s2_.Clear();
  }

 private:
  void MoveValues() {
    while (!s1_.IsEmpty()) {
      auto v = s1_.Top();
      s1_.Pop();
      s2_.Push(v);
    }
  }

 private:
  MinStack s1_;
  MinStack s2_;
};

void MakeActionWithStack(const std::string& command, MinQueue& queue) {
  if ("enqueue" == command) {
    size_t value;
    std::cin >> value;
    queue.Push(value);
    std::cout << "ok\n";
  } else if ("dequeue" == command) {
    size_t poped = queue.Front();
    queue.Pop();
    std::cout << poped << '\n';
  } else if ("front" == command) {
    std::cout << queue.Front() << '\n';
  } else if ("min" == command) {
    std::cout << queue.Min() << '\n';
  } else if ("size" == command) {
    std::cout << queue.Size() << '\n';
  } else if ("clear" == command) {
    queue.Clear();
    std::cout << "ok\n";
  }
}

void Process(int commands, MinQueue& queue) {
  std::string command;
  for (int i = 0; i < commands; ++i) {
    std::cin >> command;
    if (command == "dequeue" || command == "front" || command == "min") {
      if (queue.Size() == 0) {
        std::cout << "error" << '\n';
        continue;
      }
    }
    MakeActionWithStack(command, queue);
  }
}

int main() {
  int n_command = 0;
  std::cin >> n_command;
  MinQueue queue;
  Process(n_command, queue);
}

}  // namespace hat
