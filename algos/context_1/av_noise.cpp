#include <cmath>
#include <iomanip>
#include <iostream>

void Process(const double* arr, const size_t req) {
  for (size_t i = 0; i < req; ++i) {
    size_t left = 0;
    size_t right = 0;
    std::cin >> left >> right;
    left++;
    right++;
    double sum = arr[right] / arr[left - 1];
    std::cout << std::fixed << std::setprecision(10) << std::showpoint
              << std::pow(sum, 1.f / (double)(right - left + 1)) << std::endl;
  }
}

int main() {
  size_t n = 0;
  std::cin >> n;
  auto* arr = new double[n + 1];
  arr[0] = 1.f;
  for (size_t i = 1; i < n + 1; ++i) {
    std::cin >> arr[i];
    arr[i] *= arr[i - 1];
  }
  size_t req = 0;
  std::cin >> req;
  Process(arr, req);
  delete[] arr;
}
