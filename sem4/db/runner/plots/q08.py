# runner/plots/q_team_roles_3d.py
from __future__ import annotations
from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np

OUTPUT_DIR = Path(__file__).parent.parent.parent / "artifacts"


def build_team_roles_3d(
    rows: list[dict],
    output_path: Path,
    max_teams: int = 15,
) -> None:
    seen_teams = []
    for r in rows:
        if r["team_name"] not in seen_teams:
            seen_teams.append(r["team_name"])
        if len(seen_teams) == max_teams:
            break

    rows = [r for r in rows if r["team_name"] in seen_teams]

    teams = seen_teams
    roles = sorted(set(r["team_role"] for r in rows))

    team_idx = {name: i for i, name in enumerate(teams)}
    role_idx = {name: i for i, name in enumerate(roles)}

    team_labels = [name[:12] + "…" if len(name) > 12 else name for name in teams]

    fig = plt.figure(figsize=(14, 8))
    ax  = fig.add_subplot(111, projection="3d")

    colors = ["#378ADD", "#D85A30", "#2C6E49", "#9B59B6", "#E67E22"]

    bar_width = 0.6
    bar_depth = 0.4

    for row in rows:
        xi  = team_idx[row["team_name"]]
        yi  = role_idx[row["team_role"]]
        val = row["member_count"]

        ax.bar3d(
            xi - bar_width / 2,
            yi - bar_depth / 2,
            0,
            bar_width,
            bar_depth,
            val,
            color=colors[yi % len(colors)],
            alpha=0.85,
            shade=True,
        )

    ax.set_xticks(range(len(teams)))
    ax.set_xticklabels(team_labels, rotation=45, ha="right", fontsize=7)
    ax.set_yticks(range(len(roles)))
    ax.set_yticklabels(roles, fontsize=9)
    ax.set_zlabel("число участников", fontsize=10)
    ax.set_xlabel("команда", fontsize=10, labelpad=10)
    ax.set_ylabel("роль", fontsize=10, labelpad=10)
    ax.set_title(
        f"участники команд по ролям (первые {len(teams)})",
        fontsize=13,
        pad=15,
    )

    handles = [
        plt.Rectangle((0, 0), 1, 1, color=colors[i % len(colors)])
        for i in range(len(roles))
    ]
    ax.legend(handles, roles, loc="upper left", fontsize=9, framealpha=0.9)

    ax.view_init(elev=25, azim=-60)

    plt.tight_layout()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(output_path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  график сохранён: {output_path}")
