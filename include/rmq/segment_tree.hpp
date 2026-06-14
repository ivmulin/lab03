#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <algorithm>
#include <utility>
#include <vector>

class SegmentTree {
public:
  SegmentTree() = default;
  ~SegmentTree() = default;

  // Построение дерева. Асимптотика: O(N) операций
  long long build(const std::vector<int> &arr);

  // Запрос минимума на отрезке [l, r]. Асимптотика: O(log N) операций
  // Возвращает {Результат, Число операций}
  std::pair<int, long long> query(int l, int r);

  // Обновление элемента в позиции idx значением val. Асимптотика: O(log N)
  // операций Возвращает Число операций
  long long update(int idx, int val);

private:
  int n;
  std::vector<int> tree;

  void build_tree(const std::vector<int> &arr, int v, int tl, int tr,
                  long long &ops);
  int query_tree(int v, int tl, int tr, int l, int r, long long &ops);
  void update_tree(int v, int tl, int tr, int idx, int val, long long &ops);
};

#endif // SEGMENT_TREE_H
