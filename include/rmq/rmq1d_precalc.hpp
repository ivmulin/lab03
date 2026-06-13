#ifndef RMQ1D_PRECALC_H
#define RMQ1D_PRECALC_H

#include <utility>
#include <vector>

// === 1D RMQ (Range Minimum Query) через предпросчет всех отрезков ===
class RMQ1DPrecalc {
public:
  RMQ1DPrecalc() = default;
  ~RMQ1DPrecalc() = default;

  // Построение структуры. Возвращает число операций: O(N^2)
  long long build(const std::vector<int> &arr);

  // Запрос минимума на отрезке [l, r]. Возвращает {Результат, Число операций}:
  // O(1)
  std::pair<int, long long> query(int l, int r);

private:
  std::vector<std::vector<int>> table;
};

#endif // RMQ1D_PRECALC_H
