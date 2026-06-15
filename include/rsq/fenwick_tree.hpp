#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

#include <utility>
#include <vector>

// === RSQ через дерево Фенвика (Binary Indexed Tree) ===
// Build: O(N log N), Query: O(log N), Update: O(log N)
class FenwickTree {
public:
  FenwickTree() = default;
  ~FenwickTree() = default;

  // Построение структуры. Возвращает число операций: O(N log N)
  long long build(const std::vector<int> &arr);

  // Запрос суммы на отрезке [l, r]. Возвращает {Результат, Число операций}: O(log N)
  std::pair<long long, long long> query(int l, int r);

  // Обновление: установить arr[idx] = val. Возвращает число операций: O(log N)
  long long update(int idx, int val);

private:
  int n;
  std::vector<long long> tree;
  std::vector<int> data; // текущие значения для вычисления delta при update

  // Прибавить delta к позиции i (1-indexed). Возвращает число операций.
  long long point_add(int i, long long delta);

  // Сумма префикса [1..i] (1-indexed). Возвращает {сумма, число операций}.
  std::pair<long long, long long> prefix_sum(int i);
};

#endif // FENWICK_TREE_H
