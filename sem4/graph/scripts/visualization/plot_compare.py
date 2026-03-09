import argparse

import matplotlib.pyplot as plt

from ..core.config import plot_cfg


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('data_file')
    parser.add_argument('output_file')
    parser.add_argument('title', nargs='?', default='')
    args = parser.parse_args()

    names: list[str] = []
    iterations: list[int] = []

    with open(args.data_file) as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) == 2:
                names.append(parts[0])
                iterations.append(int(parts[1]))

    if not names:
        print("No data")
        return

    title = args.title.strip('"') or 'Сравнение алгоритмов'

    fig, ax = plt.subplots(figsize=(7, max(2, len(names) * 1.4)))
    colors = ['#4682B4', '#DC143C', '#228B22', '#FF8C00']
    bars = ax.barh(names, iterations,
                   color=[colors[i % len(colors)] for i in range(len(names))],
                   edgecolor='black', linewidth=0.8)

    for bar, val in zip(bars, iterations):
        ax.text(bar.get_width() + max(iterations) * 0.01, bar.get_y() + bar.get_height() / 2,
                str(val), va='center', fontsize=plot_cfg.title_font_size - 2,
                fontweight='bold')

    ax.set_xlabel('Количество итераций', fontsize=plot_cfg.title_font_size - 2)
    ax.set_title(title, fontsize=plot_cfg.title_font_size, fontweight=plot_cfg.title_font_weight)
    ax.set_xlim(0, max(iterations) * 1.15)
    ax.invert_yaxis()
    fig.tight_layout()
    fig.savefig(args.output_file, dpi=plot_cfg.dpi, bbox_inches='tight')
    plt.close(fig)
    print(f"[OK] Диаграмма сравнения сохранена в {args.output_file}")


if __name__ == '__main__':
    main()
