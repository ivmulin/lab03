#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "rmq/rmq1d_precalc.hpp"
#include "rmq/segment_tree.hpp"
#include "rmq/sparse_table.hpp"
#include "rmq/sqrt_decomposition.hpp"
#include "rsq/fenwick_tree.hpp"
#include "rsq/rsq1d.hpp"
#include "rsq/rsq2d.hpp"

int main() {
  const int k = 15;
  const int start_n = 100;
  const int increment_n = 100;
  const int num_queries = 1000;

  std::mt19937 rng(1337);
  std::uniform_int_distribution<int> val_dist(-1000, 1000);

  std::string result_filename = "result.csv";
  std::ofstream csv_file(result_filename);
  if (!csv_file.is_open()) {
    std::cerr << "Error: Could not open " << result_filename << " for writing!\n";
    return 1;
  }

  csv_file << "N,RSQ1D_Build,RSQ1D_Query_Avg,"
           << "RSQ2D_Build,RSQ2D_Query_Avg,"
           << "RMQ1D_Build,RMQ1D_Query_Avg,"
           << "SqrtRMQ_Build,SqrtRMQ_Query_Avg,SqrtRMQ_Update_Avg,"
           << "SqrtRSQ_Build,SqrtRSQ_Query_Avg,SqrtRSQ_Update_Avg,"
           << "SegTree_Build,SegTree_Query_Avg,SegTree_Update_Avg,"
           << "Fenwick_Build,Fenwick_Query_Avg,Fenwick_Update_Avg,"
           << "SparseTable_Build,SparseTable_Query_Avg\n";

  std::cout << "Measurements deployed" << std::endl;

  auto min_op  = [](int a, int b) { return std::min(a, b); };
  auto plus_op = [](int a, int b) { return a + b; };

  for (int step = 0; step < k; ++step) {
    int n = start_n + step * increment_n;
    std::cout << "Testing N = " << n << "...\n";

    std::vector<int> arr_1d(n);
    for (int i = 0; i < n; ++i) {
      arr_1d[i] = val_dist(rng);
    }

    std::vector<std::vector<int>> matrix_2d(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        matrix_2d[i][j] = val_dist(rng);

    std::uniform_int_distribution<int> idx_dist(0, n - 1);

    // --- RSQ 1D ---
    RSQ1D rsq1d;
    long long rsq1d_build_ops = rsq1d.build(arr_1d);
    long long rsq1d_query_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      rsq1d_query_ops_sum += rsq1d.query(l, r).second;
    }
    double rsq1d_query_avg = static_cast<double>(rsq1d_query_ops_sum) / num_queries;

    // --- RSQ 2D ---
    RSQ2D rsq2d;
    long long rsq2d_build_ops = rsq2d.build(matrix_2d);
    long long rsq2d_query_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int r1 = idx_dist(rng), c1 = idx_dist(rng), r2 = idx_dist(rng), c2 = idx_dist(rng);
      if (r1 > r2) std::swap(r1, r2);
      if (c1 > c2) std::swap(c1, c2);
      rsq2d_query_ops_sum += rsq2d.query(r1, c1, r2, c2).second;
    }
    double rsq2d_query_avg = static_cast<double>(rsq2d_query_ops_sum) / num_queries;

    // --- RMQ 1D предпросчёт ---
    RMQ1DPrecalc rmq1d;
    long long rmq1d_build_ops = rmq1d.build(arr_1d);
    long long rmq1d_query_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      rmq1d_query_ops_sum += rmq1d.query(l, r).second;
    }
    double rmq1d_query_avg = static_cast<double>(rmq1d_query_ops_sum) / num_queries;

    // --- Корневая декомпозиция (RMQ) ---
    SqrtDecomposition<int, decltype(min_op)> sqrt_decomp(INT_MAX, min_op);
    long long sqrt_build_ops = sqrt_decomp.build(arr_1d);
    long long sqrt_query_ops_sum = 0, sqrt_update_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      sqrt_query_ops_sum += sqrt_decomp.query(l, r).second;
      sqrt_update_ops_sum += sqrt_decomp.update(idx_dist(rng), val_dist(rng));
    }
    double sqrt_query_avg  = static_cast<double>(sqrt_query_ops_sum)  / num_queries;
    double sqrt_update_avg = static_cast<double>(sqrt_update_ops_sum) / num_queries;

    // --- Корневая декомпозиция (RSQ) ---
    SqrtDecomposition<int, decltype(plus_op)> sqrt_rsq(0, plus_op);
    long long sqrt_rsq_build_ops = sqrt_rsq.build(arr_1d);
    long long sqrt_rsq_query_ops_sum = 0, sqrt_rsq_update_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      sqrt_rsq_query_ops_sum += sqrt_rsq.query(l, r).second;
      sqrt_rsq_update_ops_sum += sqrt_rsq.update(idx_dist(rng), val_dist(rng));
    }
    double sqrt_rsq_query_avg  = static_cast<double>(sqrt_rsq_query_ops_sum)  / num_queries;
    double sqrt_rsq_update_avg = static_cast<double>(sqrt_rsq_update_ops_sum) / num_queries;

    // --- Дерево отрезков (RMQ) ---
    SegmentTree seg_tree;
    long long seg_build_ops = seg_tree.build(arr_1d);
    long long seg_query_ops_sum = 0, seg_update_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      seg_query_ops_sum  += seg_tree.query(l, r).second;
      seg_update_ops_sum += seg_tree.update(idx_dist(rng), val_dist(rng));
    }
    double seg_query_avg  = static_cast<double>(seg_query_ops_sum)  / num_queries;
    double seg_update_avg = static_cast<double>(seg_update_ops_sum) / num_queries;

    // --- Дерево Фенвика (RSQ) ---
    FenwickTree fenwick;
    long long fenwick_build_ops = fenwick.build(arr_1d);
    long long fenwick_query_ops_sum = 0, fenwick_update_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      fenwick_query_ops_sum  += fenwick.query(l, r).second;
      fenwick_update_ops_sum += fenwick.update(idx_dist(rng), val_dist(rng));
    }
    double fenwick_query_avg  = static_cast<double>(fenwick_query_ops_sum)  / num_queries;
    double fenwick_update_avg = static_cast<double>(fenwick_update_ops_sum) / num_queries;

    // --- Разреженная таблица (RMQ) ---
    SparseTable sparse;
    long long sparse_build_ops = sparse.build(arr_1d);
    long long sparse_query_ops_sum = 0;
    for (int q = 0; q < num_queries; ++q) {
      int l = idx_dist(rng), r = idx_dist(rng);
      if (l > r) std::swap(l, r);
      sparse_query_ops_sum += sparse.query(l, r).second;
    }
    double sparse_query_avg = static_cast<double>(sparse_query_ops_sum) / num_queries;

    csv_file << n << ","
             << rsq1d_build_ops  << "," << rsq1d_query_avg  << ","
             << rsq2d_build_ops  << "," << rsq2d_query_avg  << ","
             << rmq1d_build_ops  << "," << rmq1d_query_avg  << ","
             << sqrt_build_ops      << "," << sqrt_query_avg      << "," << sqrt_update_avg     << ","
             << sqrt_rsq_build_ops  << "," << sqrt_rsq_query_avg  << "," << sqrt_rsq_update_avg << ","
             << seg_build_ops    << "," << seg_query_avg    << "," << seg_update_avg   << ","
             << fenwick_build_ops << "," << fenwick_query_avg << "," << fenwick_update_avg << ","
             << sparse_build_ops  << "," << sparse_query_avg  << "\n";
  }

  csv_file.close();
  std::cout << "Done! Results saved to " << result_filename << "\n";
  return 0;
}
