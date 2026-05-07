from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

OUTPUT_DIR = Path(__file__).parent.parent.parent / "artifacts"


def build_histogram(rows: list[dict], output_path: Path, kde: bool = True) -> None:
    teams        = [r["team_name"] for r in rows]
    competitions = np.array([r["competition_count"] for r in rows], dtype=float)
    submissions  = np.array([r["submission_count"]  for r in rows], dtype=float)

    labels = [name[:15] + "..." if len(name) > 15 else name for name in teams]
    x      = np.arange(len(teams))
    width  = 0.35

    color_comps = "#378ADD"
    color_subms = "#D85A30"

    fig, ax = plt.subplots(figsize=(12, 5))

    ax.bar(x - width / 2, competitions, width=width,
           color=color_comps, alpha=0.85, label="соревнований")
    ax.bar(x + width / 2, submissions,  width=width,
           color=color_subms, alpha=0.85, label="попыток (submission)")

    ax.set_ylabel("количество", fontsize=11)
    ax.set_ylim(0, max(competitions.max(), submissions.max()) * 1.4)
    ax.yaxis.set_major_locator(plt.MaxNLocator(integer=True))

    ax.set_xticks(x)
    ax.set_xticklabels(labels, rotation=35, ha="right", fontsize=9)
    ax.set_xlabel("команда", fontsize=11)
    ax.set_title("первые 20 команд: соревнования и попытки", fontsize=13, pad=12)
    ax.grid(axis="y", alpha=0.3, linestyle="--")
    ax.legend(fontsize=10)

    fig.tight_layout()
    fig.savefig(output_path)
    plt.close(fig)
