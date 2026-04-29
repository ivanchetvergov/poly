from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

OUTPUT_DIR = Path(__file__).parent.parent.parent / "artifacts"


def build_histogram(rows: list[dict], output_path: Path, kde: bool = True) -> None:
    teams          = [r["team_name"] for r in rows]
    competitions   = np.array([r["competition_count"] for r in rows], dtype=float)
    submissions    = np.array([r["submission_count"] for r in rows], dtype=float)

    labels = [name[:15] + "…" if len(name) > 15 else name for name in teams]
    x      = np.arange(len(teams))
    width  = 0.35

    fig, ax1 = plt.subplots(figsize=(12, 5))

    color_comps = "#378ADD"
    color_subms = "#D85A30"

    bars1 = ax1.bar(
        x - width / 2, competitions,
        width=width, color=color_comps, alpha=0.85,
        label="соревнований",
    )
    ax1.set_ylabel("соревнований", color=color_comps, fontsize=11)
    ax1.tick_params(axis="y", labelcolor=color_comps)
    ax1.set_ylim(0, competitions.max() * 1.4)

    ax2 = ax1.twinx()
    bars2 = ax2.bar(
        x + width / 2, submissions,
        width=width, color=color_subms, alpha=0.85,
        label="попыток (submission)",
    )
    ax2.set_ylabel("попыток (submission)", color=color_subms, fontsize=11)
    ax2.tick_params(axis="y", labelcolor=color_subms)
    ax2.set_ylim(0, submissions.max() * 1.4)

    if kde:
        from scipy.stats import gaussian_kde
        xs = np.linspace(0, len(teams) - 1, 300)
        for ax, vals, color in [
            (ax1, competitions, color_comps),
            (ax2, submissions, color_subms),
        ]:
            kde_fn   = gaussian_kde(np.arange(len(teams)), bw_method=0.4)
            kde_vals = kde_fn(xs) * vals.sum() * width * 8
            ax.plot(xs, kde_vals, color=color, linewidth=2, linestyle="--")

    ax1.set_xticks(x)
    ax1.set_xticklabels(labels, rotation=35, ha="right", fontsize=9)
    ax1.set_xlabel("команда", fontsize=11)
    ax1.set_title("первые 20 команд: соревнования и попытки", fontsize=13, pad=12)
    ax1.grid(axis="y", alpha=0.3, linestyle="--")

    fig.tight_layout()
    fig.savefig(output_path)
    plt.close(fig)
