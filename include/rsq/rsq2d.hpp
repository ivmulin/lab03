#ifndef RSQ2D_H
#define RSQ2D_H

#include <utility>
#include <vector>

// === 2D RSQ (Range Sum Query Matrix) через префиксные суммы ===
class RSQ2D {
public:
  RSQ2D() = default;
  ~RSQ2D() = default;

  // Построение структуры. Возвращает число операций: O(N * M)
  long long build(const std::vector<std::vector<int>> &matrix);

  // Запрос прямоугольника от (r1, c1) до (r2, c2). Возвращает {Результат, Число
  // операций}: O(1)
  std::pair<long long, long long> query(int r1, int c1, int r2, int c2);

private:
  std::vector<std::vector<long long>> pref;
};

#endif // RSQ2D_H
