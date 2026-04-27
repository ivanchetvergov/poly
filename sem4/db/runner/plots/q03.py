from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

OUTPUT_DIR = Path(__file__).parent.parent.parent / "artifacts"


def build_histogram(rows: list[dict], output_path: Path, kde: bool = True) -> None:
    teams          = [r["team_name"] for r in rows]
    members        = np.array([r["member_count"] for r in rows], dtype=float)
    participations = np.array([r["participation_count"] for r in rows], dtype=float)

    labels = [name[:15] + "…" if len(name) > 15 else name for name in teams]
    x      = np.arange(len(teams))
    width  = 0.35

    fig, ax1 = plt.subplots(figsize=(12, 5))

    color_members = "#378ADD"
    color_parts   = "#D85A30"

    bars1 = ax1.bar(
        x - width / 2, members,
        width=width, color=color_members, alpha=0.85,
        label="участников в команде",
    )
    ax1.set_ylabel("участников в команде", color=color_members, fontsize=11)
    ax1.tick_params(axis="y", labelcolor=color_members)
    ax1.set_ylim(0, members.max() * 1.4)

    ax2 = ax1.twinx()
    bars2 = ax2.bar(
        x + width / 2, participations,
        width=width, color=color_parts, alpha=0.85,
        label="участий в соревнованиях",
    )
    ax2.set_ylabel("участий в соревнованиях", color=color_parts, fontsize=11)
    ax2.tick_params(axis="y", labelcolor=color_parts)
    ax2.set_ylim(0, participations.max() * 1.4)

    if kde:
        from scipy.stats import gaussian_kde
        xs = np.linspace(0, len(teams) - 1, 300)
        for ax, vals, color in [
            (ax1, members, color_members),
            (ax2, participations, color_parts),
        ]:
            kde_fn   = gaussian_kde(np.arange(len(teams)), bw_method=0.4)
            kde_vals = kde_fn(xs) * vals.sum() * width * 8
            ax.plot(xs, kde_vals, color=color, linewidth=2, linestyle="--")

    ax1.set_xticks(x)
    ax1.set_xticklabels(labels, rotation=35, ha="right", fontsize=9)
    ax1.set_xlabel("команда", fontsize=11)
    ax1.set_title("первые 20 команд: участники и участия", fontsize=13, pad=12)
    ax1.grid(axis="y", alpha=0.3, linestyle="--")

    fig.legend(
        handles=[bars1, bars2],
        labels=["участников в команде", "участий в соревнованиях"],
        loc="upper right",
        bbox_to_anchor=(0.98, 0.98),
        framealpha=0.9,
        fontsize=10,
    )

    plt.tight_layout()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  график сохранён: {output_path}")
