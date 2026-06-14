#ifndef SQRT_DECOMPOSITION_H
#define SQRT_DECOMPOSITION_H

#include <algorithm>
#include <cmath>
#include <functional>
#include <utility>
#include <vector>

template <typename T, typename Compare = std::function<T(T, T)>>
class SqrtDecomposition {
public:
  // Конструктор принимает нейтральный элемент (для инициализации) и бинарную
  // функцию/лямбду
  SqrtDecomposition(T neutral_value, Compare op)
      : neutral(neutral_value), operation(op), block_size(1) {}

  ~SqrtDecomposition() = default;

  // Построение структуры. Асимптотика: O(N)
  long long build(const std::vector<T> &arr) {
    data = arr;
    int n = data.size();
    block_size = std::max(1, static_cast<int>(std::sqrt(n)));
    int num_blocks = (n + block_size - 1) / block_size;
    blocks.assign(num_blocks, neutral);

    long long ops = 0;
    for (int i = 0; i < n; ++i) {
      ops++; // Шаг цикла и применение операции
      int block_idx = i / block_size;
      if (i % block_size == 0) {
        blocks[block_idx] = data[i];
      } else {
        blocks[block_idx] = operation(blocks[block_idx], data[i]);
      }
    }
    return ops;
  }

  // Запрос на отрезке [l, r]. Асимптотика: O(sqrt(N))
  std::pair<T, long long> query(int l, int r) {
    long long ops = 0;
    T result = neutral;
    bool first_assignment = true;

    int c_l = l / block_size;
    int c_r = r / block_size;

    if (c_l == c_r) {
      for (int i = l; i <= r; ++i) {
        ops++;
        if (first_assignment) {
          result = data[i];
          first_assignment = false;
        } else {
          result = operation(result, data[i]);
        }
      }
    } else {
      // Левый неполный блок
      int end_l = (c_l + 1) * block_size - 1;
      for (int i = l; i <= end_l; ++i) {
        ops++;
        if (first_assignment) {
          result = data[i];
          first_assignment = false;
        } else {
          result = operation(result, data[i]);
        }
      }
      // Полные промежуточные блоки
      for (int i = c_l + 1; i < c_r; ++i) {
        ops++;
        if (first_assignment) {
          result = blocks[i];
          first_assignment = false;
        } else {
          result = operation(result, blocks[i]);
        }
      }
      // Правый неполный блок
      for (int i = c_r * block_size; i <= r; ++i) {
        ops++;
        if (first_assignment) {
          result = data[i];
          first_assignment = false;
        } else {
          result = operation(result, data[i]);
        }
      }
    }
    return {result, ops};
  }

  // Обновление элемента в точке. Асимптотика: O(sqrt(N))
  long long update(int idx, T val) {
    long long ops = 0;
    data[idx] = val;
    int block_idx = idx / block_size;

    int start_i = block_idx * block_size;
    int end_i = std::min(static_cast<int>(data.size()), start_i + block_size);

    blocks[block_idx] = data[start_i];
    for (int i = start_i + 1; i < end_i; ++i) {
      ops++; // Пересчет всего блока заново
      blocks[block_idx] = operation(blocks[block_idx], data[i]);
    }
    return ops;
  }

private:
  int block_size;
  T neutral;
  Compare operation;
  std::vector<T> data;
  std::vector<T> blocks;
};

#endif // SQRT_DECOMPOSITION_H
