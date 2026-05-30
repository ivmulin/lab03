#include "rsq.hpp"
#include <iostream>
#include <vector>

// Возвращает сумму на подотрезке за O(1)
int RSQ1d_prefixSum(const std::vector<int> &arr, size_t l, size_t r) {
  std::vector<int> pref(arr.size() + 1, 0);
  for (size_t i = 1; i < pref.size(); i++)
    pref[i] = pref[i - 1] + arr[i - 1];

  return pref[r] - pref[l];
}

int RSQ2d_prefixSum(const std::vector<std::vector<int>> &arr, size_t l1,
                    size_t r1, size_t l2, size_t r2) {
  // Создаём матрицу префиксных сумм
  size_t n = arr.size();
  size_t m = n > 0 ? arr[0].size() : 0;
  std::vector<std::vector<int>> pref(n + 1, std::vector<int>(m + 1, 0));

  for (size_t i = 1; i < n + 1; i++) {
    for (size_t j = 1; j < m + 1; j++) {
      pref[i][j] = pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1] +
                   arr[i - 1][j - 1];
    }
  }

  std::cout << "Prefix Matrix:\n";
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= m; ++j) {
      std::cout << pref[i][j] << "\t";
    }
    std::cout << "\n";
  }

  // Исправляем границы суммирования
  int x1 = std::min(l1, l2);
  int x2 = std::max(l1, l2);
  int y1 = std::min(r1, r2);
  int y2 = std::max(r1, r2);

  int sum =
      pref[x2 + 1][y2 + 1] - pref[x2 + 1][y1] - pref[x1][y2 + 1] + pref[x1][y1];
  return sum;
}
