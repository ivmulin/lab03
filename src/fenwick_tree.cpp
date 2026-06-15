#include "rsq/fenwick_tree.hpp"

long long FenwickTree::point_add(int i, long long delta) {
  long long ops = 0;
  while (i <= n) {
    ops++;
    tree[i] += delta;
    i += i & (-i); // переход к следующему ответственному узлу
  }
  return ops;
}

std::pair<long long, long long> FenwickTree::prefix_sum(int i) {
  long long sum = 0;
  long long ops = 0;
  while (i > 0) {
    ops++;
    sum += tree[i];
    i -= i & (-i); // переход к предыдущему непересекающемуся отрезку
  }
  return {sum, ops};
}

// Построение: N вызовов point_add -> O(N log N) операций
long long FenwickTree::build(const std::vector<int> &arr) {
  n = arr.size();
  tree.assign(n + 1, 0);
  data = arr;

  long long ops = 0;
  for (int i = 0; i < n; ++i) {
    ops += point_add(i + 1, arr[i]); // 1-indexed
  }
  return ops;
}

// Запрос суммы [l, r] (0-indexed): prefix(r+1) - prefix(l)
std::pair<long long, long long> FenwickTree::query(int l, int r) {
  auto [sum_r, ops_r] = prefix_sum(r + 1);
  auto [sum_l, ops_l] = prefix_sum(l);
  return {sum_r - sum_l, ops_r + ops_l};
}

// Установить arr[idx] = val (0-indexed): добавляем delta = val - old_val
long long FenwickTree::update(int idx, int val) {
  long long delta = (long long)val - data[idx];
  data[idx] = val;
  return point_add(idx + 1, delta); // 1-indexed
}
