#include "rsq/rsq1d.hpp"

// Построение: O(N) операций
long long RSQ1D::build(const std::vector<int> &arr) {
  int n = arr.size();
  pref.assign(n + 1, 0);
  long long ops = 0;
  for (int i = 0; i < n; ++i) {
    ops++; // Шаг цикла и обращение к памяти
    pref[i + 1] = pref[i] + arr[i];
  }
  return ops;
}

// Запрос: O(1) операций
std::pair<long long, long long> RSQ1D::query(int l, int r) {
  long long ops = 2; // Два обращения к массиву префиксов
  long long sum = pref[r + 1] - pref[l];
  return {sum, ops};
}
