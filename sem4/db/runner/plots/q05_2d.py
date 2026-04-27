from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


def build_q05_2d(rows: list[dict], output_path: Path) -> None:
    x = np.array([r["tm_count"] for r in rows], dtype=float)
    y = np.array([r["user_count"] for r in rows], dtype=float)

    order = np.argsort(x)
    x = x[order]
    y = y[order]

    fig, ax = plt.subplots(figsize=(10, 6))

    ax.plot(
        x,
        y,
        color="#2C6E49",
        linewidth=2,
        alpha=0.9,
        label="распределение пользователей",
    )
    ax.scatter(
        x,
        y,
        s=55,
        color="#D68C45",
        edgecolors="white",
        linewidth=0.8,
        zorder=3,
        label="точки (tm_count, user_count)",
    )

    for xi, yi in zip(x, y):
        if yi >= max(y) * 0.55:
            ax.annotate(
                f"{int(yi)}",
                (xi, yi),
                textcoords="offset points",
                xytext=(0, 6),
                ha="center",
                fontsize=8,
            )

    ax.set_title("q05: пользователи по числу командных участий", fontsize=13, pad=12)
    ax.set_xlabel("число участий в командах (tm_count)", fontsize=11)
    ax.set_ylabel("число пользователей (user_count)", fontsize=11)
    ax.grid(alpha=0.3, linestyle="--")
    ax.set_xticks(x)
    ax.legend(loc="upper right", framealpha=0.9)

    plt.tight_layout()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  график сохранён: {output_path}")
