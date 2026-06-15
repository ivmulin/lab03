#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H

#include <utility>
#include <vector>

// === RMQ через разреженную таблицу (Sparse Table) ===
// Build: O(N log N), Query: O(1) через перекрывающиеся отрезки
// Обновление не поддерживается (статическая структура)
class SparseTable {
public:
  SparseTable() = default;
  ~SparseTable() = default;

  // Построение структуры. Возвращает число операций: O(N log N)
  long long build(const std::vector<int> &arr);

  // Запрос минимума на отрезке [l, r]. Возвращает {Результат, Число операций}: O(1)
  std::pair<int, long long> query(int l, int r);

private:
  int n;
  std::vector<std::vector<int>> table; // table[j][i] = min на [i, i+2^j-1]
  std::vector<int> log2_floor;         // предпосчитанные значения floor(log2(i))
};

#endif // SPARSE_TABLE_H
