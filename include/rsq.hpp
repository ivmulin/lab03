#pragma once
#include <cmath>
#include <vector>

// Возвращает сумму на подотрезке за O(1)
int RSQ1d_prefixSum(const std::vector<int> &arr, size_t l, size_t r);

int RSQ2d_prefixSum(const std::vector<std::vector<int>> &arr, size_t l1,
                    size_t r1, size_t l2, size_t r2);

template <typename T, typename Op> class SqrtDecomposition {
private:
  std::vector<T> src;
  std::vector<T> blocks;
  size_t block_size;
  Op op;
  T neutral_value;

public:
  SqrtDecomposition(const std::vector<T> &arr, Op operation, T neutral)
      : src(arr), op(operation), neutral_value(neutral) {

    size_t n = src.size();
    if (n == 0) {
      block_size = 1;
      return;
    }
    block_size =
        std::max(static_cast<size_t>(1), static_cast<size_t>(std::sqrt(n)));

    size_t blocks_count = (n + block_size - 1) / block_size;
    blocks.assign(blocks_count, neutral_value);

    // ИСПРАВЛЕНО: Безопасное заполнение блоков с проверкой границ
    for (size_t i = 0; i < n; ++i) {
      size_t block_idx = i / block_size;
      blocks[block_idx] = op(blocks[block_idx], src[i]);
    }
  }

  void
  update(size_t idx,
         T new_val) { // ИСПРАВЛЕНО: int заменен на size_t для консистентности
    src[idx] = new_val;

    size_t block_idx = idx / block_size;
    size_t start = block_idx * block_size;
    size_t end = std::min(src.size(), start + block_size);

    blocks[block_idx] = neutral_value;
    for (size_t i = start; i < end; ++i) {
      blocks[block_idx] = op(blocks[block_idx], src[i]);
    }
  }

  T query(size_t L, size_t R) const { // ИСПРАВЛЕНО: Заглавные буквы L и R
    T result = neutral_value;

    size_t l_block = L / block_size;
    size_t r_block = R / block_size;

    // ИСПРАВЛЕНО: Если запрос целиком внутри одного блока
    if (l_block == r_block) {
      for (size_t i = L; i <= R; ++i) {
        result = op(result, src[i]);
      }
      return result;
    }

    // 1. Левый хвост: от L до конца его блока
    size_t l_end = (l_block + 1) * block_size;
    for (size_t i = L; i < l_end; ++i) {
      result = op(result, src[i]);
    }

    // 2. Целые промежуточные блоки (границы изменены на корректные)
    for (size_t i = l_block + 1; i < r_block; ++i) {
      result = op(result, blocks[i]);
    }

    // 3. Правый хвост: от начала его блока до R (ИСПРАВЛЕНО: идет слева направо
    // во избежание вечного цикла)
    size_t r_start = r_block * block_size;
    for (size_t i = r_start; i <= R; ++i) {
      result = op(result, src[i]);
    }

    return result;
  }
};
