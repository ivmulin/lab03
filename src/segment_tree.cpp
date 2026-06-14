#include "rmq/segment_tree.hpp"
#include <climits>

long long SegmentTree::build(const std::vector<int> &arr) {
  n = arr.size();
  tree.assign(4 * n, INT_MAX);
  long long ops = 0;
  if (n > 0) {
    build_tree(arr, 1, 0, n - 1, ops);
  }
  return ops;
}

void SegmentTree::build_tree(const std::vector<int> &arr, int v, int tl, int tr,
                             long long &ops) {
  ops++; // Итерация / вызов функции
  if (tl == tr) {
    tree[v] = arr[tl];
    return;
  }
  int tm = tl + (tr - tl) / 2;
  build_tree(arr, v * 2, tl, tm, ops);
  build_tree(arr, v * 2 + 1, tm + 1, tr, ops);
  tree[v] = std::min(tree[v * 2], tree[v * 2 + 1]);
  ops++; // Сравнение и запись минимума
}

std::pair<int, long long> SegmentTree::query(int l, int r) {
  long long ops = 0;
  int res = query_tree(1, 0, n - 1, l, r, ops);
  return {res, ops};
}

int SegmentTree::query_tree(int v, int tl, int tr, int l, int r,
                            long long &ops) {
  ops++; // Проверка условий ветвления
  if (l > r) {
    return INT_MAX;
  }
  if (l == tl && r == tr) {
    return tree[v];
  }
  int tm = tl + (tr - tl) / 2;
  int left_res = query_tree(v * 2, tl, tm, l, std::min(r, tm), ops);
  int right_res =
      query_tree(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, ops);
  ops++; // Финальное сравнение на выходе из ветвления
  return std::min(left_res, right_res);
}

long long SegmentTree::update(int idx, int val) {
  long long ops = 0;
  update_tree(1, 0, n - 1, idx, val, ops);
  return ops;
}

void SegmentTree::update_tree(int v, int tl, int tr, int idx, int val,
                              long long &ops) {
  ops++; // Вызов функции и ветвление
  if (tl == tr) {
    tree[v] = val;
    return;
  }
  int tm = tl + (tr - tl) / 2;
  if (idx <= tm) {
    update_tree(v * 2, tl, tm, idx, val, ops);
  } else {
    update_tree(v * 2 + 1, tm + 1, tr, idx, val, ops);
  }
  tree[v] = std::min(tree[v * 2], tree[v * 2 + 1]);
  ops++; // Пересчет значения внутренней вершины
}
