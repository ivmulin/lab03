#include "rsq/rsq2d.hpp"

// Построение: O(N * M) операций
long long RSQ2D::build(const std::vector<std::vector<int>> &matrix) {
  if (matrix.empty() || matrix[0].empty())
    return 0;
  int n = matrix.size();
  int m = matrix[0].size();
  pref.assign(n + 1, std::vector<long long>(m + 1, 0));
  long long ops = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      ops++; // Вычисление одной ячейки префиксной матрицы
      pref[i + 1][j + 1] =
          matrix[i][j] + pref[i][j + 1] + pref[i + 1][j] - pref[i][j];
    }
  }
  return ops;
}

// Запрос: O(1) операций
std::pair<long long, long long> RSQ2D::query(int r1, int c1, int r2, int c2) {
  long long ops = 4; // Четыре обращения к префиксной матрице
  long long sum =
      pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
  return {sum, ops};
}
