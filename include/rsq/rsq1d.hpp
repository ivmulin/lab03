#ifndef RSQ1D_H
#define RSQ1D_H

#include <utility>
#include <vector>

// === 1D RSQ (Range Sum Query) через префиксные суммы ===
class RSQ1D {
public:
  RSQ1D() = default;
  ~RSQ1D() = default;

  // Построение структуры. Возвращает число операций: O(N)
  long long build(const std::vector<int> &arr);

  // Запрос суммы на отрезке [l, r]. Возвращает {Результат, Число операций}:
  // O(1)
  std::pair<long long, long long> query(int l, int r);

private:
  std::vector<long long> pref;
};

#endif // RSQ1D_H
