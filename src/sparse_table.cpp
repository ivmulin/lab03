#include "rmq/sparse_table.hpp"
#include <algorithm>
#include <climits>
#include <cmath>

// Построение: O(N log N) операций
long long SparseTable::build(const std::vector<int> &arr) {
  n = arr.size();
  if (n == 0) return 0;

  int LOG = 1;
  while ((1 << LOG) <= n) ++LOG;

  // Предподсчёт floor(log2(i)) для i от 1 до n
  log2_floor.assign(n + 1, 0);
  for (int i = 2; i <= n; ++i) {
    log2_floor[i] = log2_floor[i / 2] + 1;
  }

  // table[j][i] = min на отрезке [i, i + 2^j - 1]
  table.assign(LOG, std::vector<int>(n, INT_MAX));

  long long ops = 0;

  // Базовый слой: отрезки длины 1
  for (int i = 0; i < n; ++i) {
    ops++;
    table[0][i] = arr[i];
  }

  // Последующие слои: комбинируем два соседних подотрезка
  for (int j = 1; j < LOG; ++j) {
    int half = 1 << (j - 1);
    for (int i = 0; i + (1 << j) - 1 < n; ++i) {
      ops++;
      table[j][i] = std::min(table[j - 1][i], table[j - 1][i + half]);
    }
  }

  return ops;
}

// Запрос минимума [l, r]: два перекрывающихся отрезка длины 2^k, O(1)
std::pair<int, long long> SparseTable::query(int l, int r) {
  long long ops = 3; // два чтения из таблицы + одно сравнение
  int k = log2_floor[r - l + 1];
  int result = std::min(table[k][l], table[k][r - (1 << k) + 1]);
  return {result, ops};
}
