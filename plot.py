import os
import sys

# 1. Сначала импортируем только базовый модуль matplotlib
import matplotlib
# 2. Немедленно переключаем бэкенд на генерацию файлов без GUI
matplotlib.use('Agg')

# 3. И только ТЕПЕРЬ импортируем pyplot и все остальные тяжелые библиотеки
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from scipy.interpolate import make_interp_spline

def main():
    # 1. Определение путей к файлам
    if len(sys.argv) > 1:
        csv_path = sys.argv[1]
    else:
        # По умолчанию ищем в папке build/ относительно места запуска скрипта
        csv_path = os.path.join(os.path.dirname(__file__), 'build', 'result.csv')

    if not os.path.exists(csv_path):
        print(f"Ошибка: Файл данных '{csv_path}' не найден!")
        print("Убедитесь, что вы скомпилировали и запустили C++ бинарник, либо укажите путь к CSV аргументом.")
        sys.exit(1)

    output_dir = os.path.dirname(csv_path)

    # 2. Чтение данных
    df = pd.read_csv(csv_path)
    
    # Очищаем имена столбцов от возможных пробелов
    df.columns = df.columns.str.strip()

    # Извлекаем базовые векторы данных
    N = df['N'].values
    
    # Создаем сетку для гладких кривых (интерполяция)
    N_smooth = np.linspace(N.min(), N.max(), 300)

    def plot_smooth(ax, x, y, label, marker, color):
        """Вспомогательная функция для построения сглаженных линий"""
        spline = make_interp_spline(x, y, k=3)
        y_smooth = spline(N_smooth)
        # Ограничиваем снизу нулем, чтобы интерполяция не уходила в минус
        y_smooth = np.clip(y_smooth, 0, None)
        
        # Рисуем сглаженную линию
        ax.plot(N_smooth, y_smooth, label=label, color=color, linestyle='-', linewidth=2)
        # Рисуем реальные точки замера
        ax.scatter(x, y, color=color, marker=marker, s=40, zorder=5)

    # =========================================================================
    # ГРАФИК 1: Сложность построения (Build Complexity)
    # =========================================================================
    fig, ax = plt.subplots(figsize=(11, 7))
    
    plot_smooth(ax, N, df['RSQ1D_Build'], 'RSQ 1D ($O(N)$)', 'o', '#1f77b4')
    plot_smooth(ax, N, df['RSQ2D_Build'], 'RSQ 2D ($O(N^2)$)', 's', '#ff7f0e')
    plot_smooth(ax, N, df['RMQ1D_Build'], 'RMQ 1D Precalc ($O(N^2)$)', '^', '#2ca02c')
    plot_smooth(ax, N, df['Sqrt_Build'], 'Sqrt Deco ($O(N)$)', 'v', '#d62728')
    plot_smooth(ax, N, df['SegTree_Build'], 'Segment Tree ($O(N)$)', 'd', '#9467bd')

    ax.set_title('Сложность построения структур данных (Ключевые операции)', fontsize=14, pad=15)
    ax.set_xlabel('Размер входных данных ($N$)', fontsize=12)
    ax.set_ylabel('Количество операций', fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=11, loc='upper left')
    
    plt.tight_layout()
    build_plot_path = os.path.join(output_dir, 'build_complexity.png')
    plt.savefig(build_plot_path, dpi=300)
    plt.close()

    # =========================================================================
    # ГРАФИК 2: Сложность запросов (Query Complexity)
    # =========================================================================
    fig, ax = plt.subplots(figsize=(11, 7))

    plot_smooth(ax, N, df['RSQ1D_Query_Avg'], 'RSQ 1D ($O(1)$)', 'o', '#1f77b4')
    plot_smooth(ax, N, df['RSQ2D_Query_Avg'], 'RSQ 2D ($O(1)$)', 's', '#ff7f0e')
    plot_smooth(ax, N, df['RMQ1D_Query_Avg'], 'RMQ 1D Precalc ($O(1)$)', '^', '#2ca02c')
    plot_smooth(ax, N, df['Sqrt_Query_Avg'], 'Sqrt Deco ($O(\\sqrt{N})$)', 'v', '#d62728')
    plot_smooth(ax, N, df['SegTree_Query_Avg'], 'Segment Tree ($O(\\log N)$)', 'd', '#9467bd')

    ax.set_title('Средняя сложность обработки запроса (Ключевые операции)', fontsize=14, pad=15)
    ax.set_xlabel('Размер входных данных ($N$)', fontsize=12)
    ax.set_ylabel('Количество операций на запрос', fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=11, loc='upper left')
    
    plt.tight_layout()
    query_plot_path = os.path.join(output_dir, 'query_complexity.png')
    plt.savefig(query_plot_path, dpi=300)
    plt.close()

    # =========================================================================
    # ГРАФИК 3: Сложность обновления в точке (Update Complexity)
    # =========================================================================
    fig, ax = plt.subplots(figsize=(11, 7))

    # Обновление поддерживают только Sqrt-декомпозиция и Дерево отрезков
    plot_smooth(ax, N, df['Sqrt_Update_Avg'], 'Sqrt Deco ($O(\\sqrt{N})$)', 'v', '#d62728')
    plot_smooth(ax, N, df['SegTree_Update_Avg'], 'Segment Tree ($O(\\log N)$)', 'd', '#9467bd')

    ax.set_title('Средняя сложность операции обновления в точке (Ключевые операции)', fontsize=14, pad=15)
    ax.set_xlabel('Размер входных данных ($N$)', fontsize=12)
    ax.set_ylabel('Количество операций на обновление', fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=11, loc='upper left')
    
    plt.tight_layout()
    update_plot_path = os.path.join(output_dir, 'update_complexity.png')
    plt.savefig(update_plot_path, dpi=300)
    plt.close()

    print("Графики успешно сгенерированы и сохранены:")
    print(f"  1. {build_plot_path}")
    print(f"  2. {query_plot_path}")
    print(f"  3. {update_plot_path}")

if __name__ == '__main__':
    main()
