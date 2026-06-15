import os
import sys

import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from scipy.interpolate import make_interp_spline

def main():
    if len(sys.argv) > 1:
        csv_path = sys.argv[1]
    else:
        csv_path = os.path.join(os.path.dirname(__file__), 'build', 'result.csv')

    if not os.path.exists(csv_path):
        print(f"Ошибка: Файл данных '{csv_path}' не найден!")
        print("Убедитесь, что вы скомпилировали и запустили C++ бинарник.")
        sys.exit(1)

    output_dir = os.path.dirname(csv_path)
    df = pd.read_csv(csv_path)
    df.columns = df.columns.str.strip()

    N = df['N'].values
    N_smooth = np.linspace(N.min(), N.max(), 300)

    def plot_smooth(ax, x, y, label, marker, color):
        spline = make_interp_spline(x, y, k=3)
        y_smooth = np.clip(spline(N_smooth), 0, None)
        ax.plot(N_smooth, y_smooth, label=label, color=color, linestyle='-', linewidth=2)
        ax.scatter(x, y, color=color, marker=marker, s=40, zorder=5)

    # =========================================================================
    # ГРАФИК 1: Сложность построения (Build Complexity)
    # =========================================================================
    fig, ax = plt.subplots(figsize=(12, 7))

    plot_smooth(ax, N, df['RSQ1D_Build'],       'RSQ 1D префикс ($O(N)$)',               'o', '#1f77b4')
    plot_smooth(ax, N, df['RSQ2D_Build'],       'RSQ 2D префикс ($O(N^2)$)',              's', '#ff7f0e')
    plot_smooth(ax, N, df['RMQ1D_Build'],       'RMQ 1D предпросчёт ($O(N^2)$)',          '^', '#2ca02c')
    plot_smooth(ax, N, df['SqrtRMQ_Build'],     'Корневая дек. RMQ ($O(N)$)',             'v', '#d62728')
    plot_smooth(ax, N, df['SqrtRSQ_Build'],     'Корневая дек. RSQ ($O(N)$)',             '<', '#bcbd22')
    plot_smooth(ax, N, df['SegTree_Build'],     'Дерево отрезков ($O(N)$)',               'd', '#9467bd')
    plot_smooth(ax, N, df['Fenwick_Build'],     'Дерево Фенвика ($O(N \\log N)$)',        'P', '#8c564b')
    plot_smooth(ax, N, df['SparseTable_Build'], 'Разреженная таблица ($O(N \\log N)$)',   'X', '#e377c2')

    ax.set_title('Сложность построения структур данных (ключевые операции)', fontsize=14, pad=15)
    ax.set_xlabel('Размер входных данных ($N$)', fontsize=12)
    ax.set_ylabel('Количество операций', fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=10, loc='upper left')
    plt.tight_layout()
    path = os.path.join(output_dir, 'build_complexity.png')
    plt.savefig(path, dpi=300)
    plt.close()
    print(f"  1. {path}")

    # =========================================================================
    # ГРАФИК 2: Сложность запросов (Query Complexity)
    # =========================================================================
    fig, ax = plt.subplots(figsize=(12, 7))

    plot_smooth(ax, N, df['RSQ1D_Query_Avg'],       'RSQ 1D префикс ($O(1)$)',               'o', '#1f77b4')
    plot_smooth(ax, N, df['RSQ2D_Query_Avg'],       'RSQ 2D префикс ($O(1)$)',                's', '#ff7f0e')
    plot_smooth(ax, N, df['RMQ1D_Query_Avg'],       'RMQ 1D предпросчёт ($O(1)$)',            '^', '#2ca02c')
    plot_smooth(ax, N, df['SqrtRMQ_Query_Avg'],     'Корневая дек. RMQ ($O(\\sqrt{N})$)',     'v', '#d62728')
    plot_smooth(ax, N, df['SqrtRSQ_Query_Avg'],     'Корневая дек. RSQ ($O(\\sqrt{N})$)',     '<', '#bcbd22')
    plot_smooth(ax, N, df['SegTree_Query_Avg'],     'Дерево отрезков ($O(\\log N)$)',          'd', '#9467bd')
    plot_smooth(ax, N, df['Fenwick_Query_Avg'],     'Дерево Фенвика ($O(\\log N)$)',           'P', '#8c564b')
    plot_smooth(ax, N, df['SparseTable_Query_Avg'], 'Разреженная таблица ($O(1)$)',            'X', '#e377c2')

    ax.set_title('Средняя сложность запроса на отрезке (ключевые операции)', fontsize=14, pad=15)
    ax.set_xlabel('Размер входных данных ($N$)', fontsize=12)
    ax.set_ylabel('Операций на запрос', fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=10, loc='upper left')
    plt.tight_layout()
    path = os.path.join(output_dir, 'query_complexity.png')
    plt.savefig(path, dpi=300)
    plt.close()
    print(f"  2. {path}")

    # =========================================================================
    # ГРАФИК 3: Сложность обновления в точке (Update Complexity)
    # Поддерживают: корневая декомпозиция, дерево отрезков, дерево Фенвика
    # =========================================================================
    fig, ax = plt.subplots(figsize=(12, 7))

    plot_smooth(ax, N, df['SqrtRMQ_Update_Avg'], 'Корневая дек. RMQ ($O(\\sqrt{N})$)', 'v', '#d62728')
    plot_smooth(ax, N, df['SqrtRSQ_Update_Avg'], 'Корневая дек. RSQ ($O(\\sqrt{N})$)', '<', '#bcbd22')
    plot_smooth(ax, N, df['SegTree_Update_Avg'],  'Дерево отрезков ($O(\\log N)$)',     'd', '#9467bd')
    plot_smooth(ax, N, df['Fenwick_Update_Avg'],  'Дерево Фенвика ($O(\\log N)$)',      'P', '#8c564b')

    ax.set_title('Средняя сложность обновления в точке (ключевые операции)', fontsize=14, pad=15)
    ax.set_xlabel('Размер входных данных ($N$)', fontsize=12)
    ax.set_ylabel('Операций на обновление', fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=10, loc='upper left')
    plt.tight_layout()
    path = os.path.join(output_dir, 'update_complexity.png')
    plt.savefig(path, dpi=300)
    plt.close()
    print(f"  3. {path}")

    print("Графики успешно сгенерированы.")

if __name__ == '__main__':
    print("Генерирую графики:")
    main()
