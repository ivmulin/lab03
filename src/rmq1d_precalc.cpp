#include "rmq/rmq1d_precalc.hpp"

// Построение: O(N^2) операций
long long RMQ1DPrecalc::build(const std::vector<int> &arr) {
  int n = arr.size();
  table.assign(n, std::vector<int>(n, 0));
  long long ops = 0;

  for (int i = 0; i < n; ++i) {
    int current_min = arr[i];
    for (int j = i; j < n; ++j) {
      ops++; // Сравнение и запись минимума для текущего отрезка
      if (arr[j] < current_min) {
        current_min = arr[j];
      }
      table[i][j] = current_min;
    }
  }
  return ops;
}

// Запрос: O(1) операций
std::pair<int, long long> RMQ1DPrecalc::query(int l, int r) {
  long long ops = 1; // Одно прямое чтение из таблицы ответов
  return {table[l][r], ops};
}
