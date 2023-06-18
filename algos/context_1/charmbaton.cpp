#include <cctype>
#include <iostream>

template <typename T>
class Stack {
 public:
  Stack() : values_(new T[1000]), capacity_(1000) {}

  void Push(T value) {
    if (sz_ == capacity_) {
      auto* new_values = new T[capacity_ * 2];
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

  [[nodiscard]] T Top() const { return values_[sz_ - 1]; }

  [[nodiscard]] size_t Size() const { return sz_; }

  void Clear() { sz_ = 0; }

  ~Stack() { delete[] values_; }

 private:
  T* values_;
  size_t sz_ = 0;
  size_t capacity_ = 0;
};

void PushIfReady(Stack<int>& numbers, std::string& num) {
  if (!num.empty()) {
    numbers.Push(std::atoi(num.c_str()));
    num.clear();
  }
}

void Process(Stack<int>& numbers, std::string& number,
             const std::string& input) {
  for (auto ch : input) {
    if (std::isspace(ch) != 0) {
      PushIfReady(numbers, number);
      continue;
    }
    if (std::isdigit(ch) != 0) {
      number.push_back(ch);
    } else {
      PushIfReady(numbers, number);
      int res = numbers.Top();
      numbers.Pop();
      switch (ch) {
        case '+':
          res += numbers.Top();
          break;
        case '-':
          res = numbers.Top() - res;
          break;
        case '*':
          res *= numbers.Top();
          break;
      }
      numbers.Pop();
      numbers.Push(res);
    }
  }
}

int main() {
  std::string input;
  std::getline(std::cin, input);
  std::string number;
  Stack<int> numbers;
  Process(numbers, number, input);
  std::cout << numbers.Top();
}
