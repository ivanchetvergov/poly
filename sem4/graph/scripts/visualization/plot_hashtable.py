import sys
from collections import defaultdict

import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, ArrowStyle

from scripts.core.config import plot_cfg, label_cfg, node_cfg, edge_cfg


def _read_entries(path: str):
    """Parse input lines of the form: word hash bucket."""
    entries = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 3:
                continue
            word, h, bucket = parts
            entries.append((word, int(h), int(bucket)))
    return entries


def _layout(entries):
    """Compute y-positions for buckets and nodes."""
    bucket_order = sorted({b for *_, b in entries})
    bucket_y = {b: -i for i, b in enumerate(bucket_order)}

    buckets = defaultdict(list)
    for word, h, b in entries:
        buckets[b].append((word, h))

    return bucket_order, bucket_y, buckets


def _draw_box(ax, xy, text, color, width=0.5, height=0.3):
    x, y = xy
    box = FancyBboxPatch(
        (x - width / 2, y - height / 2),
        width,
        height,
        boxstyle="round,pad=0.08",
        linewidth=1.4,
        edgecolor="black",
        facecolor=color,
    )
    ax.add_patch(box)
    ax.text(
        x,
        y,
        text,
        ha="center",
        va="center",
        fontsize=label_cfg.label_font_size,
        fontweight=label_cfg.label_font_weight,
        color=label_cfg.label_font_color,
    )


def _draw_arrow(ax, start, end):
    ax.annotate(
        "",
        xy=end,
        xytext=start,
        arrowprops=dict(
            arrowstyle=ArrowStyle(
                "simple",
                head_length=1.0,
                head_width=1.2,
                tail_width=0.15,
            ),
            linewidth=0.1,
            color='black',
            connectionstyle=edge_cfg.connection_style,
        ),
    )


def main():
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "HashTable Visualization"

    entries = _read_entries(data_file)

    bucket_order = sorted({b for *_, b in entries})
    buckets = defaultdict(list)
    for word, h, b in entries:
        buckets[b].append((word, h))

    word_set = set()
    for chain in buckets.values():
        for word, _ in chain:
            word_set.add(word)
    word_list = sorted(word_set)

    num_words = len(word_list)
    num_buckets = len(bucket_order)
    max_chain_len = max((len(chain) for chain in buckets.values()), default=0)

    spacing = 0.5

    total_h_words = (num_words - 1) * spacing if num_words > 1 else 0
    y_words_start = total_h_words / 2
    word_positions = {word: y_words_start - i * spacing for i, word in enumerate(word_list)}

    total_h_buckets = (num_buckets - 1) * spacing if num_buckets > 1 else 0
    y_buckets_start = total_h_buckets / 2
    bucket_y = {b: y_buckets_start - i * spacing for i, b in enumerate(bucket_order)}

    figsize = (plot_cfg.figsize[0] * 0.8, plot_cfg.figsize[1] * 0.55)
    fig, ax = plt.subplots(figsize=figsize, constrained_layout=True)
    ax.axis("off")

    # Calculate dynamic widths
    max_len_words = max((len(word) for word in word_list), default=0)
    max_len_buckets = max((len(str(b)) for b in bucket_order), default=0)
    max_len_chains = max((len(word) for chain in buckets.values() for word, _ in chain), default=0)
    width_words = 0.5 + max_len_words * 0.08
    width_buckets = 0.5 + max_len_buckets * 0.08
    width_chains = 0.5 + max_len_chains * 0.08

    x_word = 0.5
    x_bucket = x_word + width_words / 2 + 0.5 + width_buckets / 2
    x_chain_start = x_bucket + width_buckets / 2 + 0.5 + width_chains / 2
    dx_chain = width_chains + 0.2

    for word in word_list:
        y = word_positions[word]
        _draw_box(ax, (x_word, y), word, color=node_cfg.node_color, width=width_words)

    for b in bucket_order:
        y = bucket_y[b]
        _draw_box(ax, (x_bucket, y), f"{b}", color="#b6e3a8", width=width_buckets)

    max_chain_len = max((len(chain) for chain in buckets.values()), default=1)
    for b in bucket_order:
        chain = buckets[b]
        y = bucket_y[b]
        prev_pos = (x_bucket + width_buckets / 2, y)
        for i, (word, h) in enumerate(chain):
            x = x_chain_start + i * dx_chain
            node_label = f"{word}"
            _draw_box(ax, (x, y), node_label, color="#ffe0b2", width=width_chains, height=0.25)
            if i == 0:
                _draw_arrow(ax, prev_pos, (x - width_chains / 2, y))

    for word, h, b in entries:
        if word not in word_positions:
            continue
        wy = word_positions[word]
        by = bucket_y[b]
        _draw_arrow(ax, (x_word + width_words / 2, wy), (x_bucket - width_buckets / 2, by))

    ys = list(bucket_y.values()) + list(word_positions.values())
    min_y = min(ys)
    max_y = max(ys)
    range_y = max_y - min_y
    desired_range = range_y / 0.7
    center = (min_y + max_y) / 2
    ylim_bottom = center - desired_range / 2
    ylim_top = center + desired_range / 2
    ax.set_ylim(ylim_bottom, ylim_top)
    # header_y = ylim_top - 0.1
    # ax.text(x_word, header_y, "words", ha="center", va="bottom", fontsize=label_cfg.label_font_size, fontweight=label_cfg.label_font_weight)
    # ax.text(x_bucket, header_y, "buckets (hash)", ha="center", va="bottom", fontsize=label_cfg.label_font_size, fontweight=label_cfg.label_font_weight)
    # ax.text(x_chain_start + (max_chain_len - 1) * dx_chain / 2, header_y, "chains", ha="center", va="bottom", fontsize=label_cfg.label_font_size, fontweight=label_cfg.label_font_weight)

    ax.set_xlim(-0.6, x_chain_start + max_chain_len * dx_chain + 0.6)

    fig.savefig(output_file, dpi=plot_cfg.dpi, bbox_inches="tight")
    plt.show()
    plt.close(fig)

if __name__ == '__main__':
    main()
