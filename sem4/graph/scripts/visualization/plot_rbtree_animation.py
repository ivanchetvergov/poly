import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import networkx as nx
from matplotlib.lines import Line2D
import os

from scripts.core.renderer import Renderer
from scripts.core.config import plot_cfg, node_cfg, edge_cfg, label_cfg
from .plot_rbtree import hierarchy_pos

def read_snapshots(filename):
    snapshots = []
    with open(filename, 'r') as f:
        content = f.read()
    parts = content.split('---\n')
    for i, part in enumerate(parts):
        if not part.strip():
            continue
        lines = part.strip().split('\n')
        step = int(lines[0])
        data = '\n'.join(lines[1:])
        snapshots.append((step, data))
    return snapshots

def create_graph_from_data(data):
    G = nx.DiGraph()
    node_colors = {}
    for line in data.split('\n'):
        raw = line.strip()
        if not raw:
            continue
        # Preferred format: node<TAB>parent<TAB>color.
        if '\t' in raw:
            fields = raw.split('\t')
            if len(fields) != 3:
                continue
            node, parent, color = fields
        else:
            # Backward-compat fallback for old format.
            parts = raw.split()
            if len(parts) < 3:
                continue
            parent = parts[-2]
            color = parts[-1]
            node = ' '.join(parts[:-2])

        G.add_node(node)
        node_colors[node] = 'red' if color == 'RED' else 'black'
        if parent != 'null':
            G.add_edge(parent, node)
    return G, node_colors

def animate_rbtree_growth(snapshots, output_file='assets/gif/rbtree_growth.gif'):
    if not snapshots:
        print("No snapshots")
        return

    fig = plt.figure(figsize=(plot_cfg.figsize))
    ax = fig.add_axes([0, 0, 1, 1])

    def update(frame):
        ax.clear()
        step, data = snapshots[frame]
        G, node_colors = create_graph_from_data(data)
        if not G.nodes():
            return

        roots = [n for n in G.nodes() if G.in_degree(n) == 0]
        if not roots:
            roots = [next(iter(G.nodes()))]

        # Position all components so each node has coordinates.
        pos = {}
        span = 4.0
        for i, root in enumerate(roots):
            xcenter = i * (span + 1.0)
            pos.update(hierarchy_pos(G, root, width=span, vert_gap=0.5, vert_loc=1, xcenter=xcenter))

        # Safety fallback for any unplaced nodes.
        missing = [n for n in G.nodes() if n not in pos]
        for i, node in enumerate(missing):
            pos[node] = (len(roots) * (span + 1.0) + 1.0, 1 - i * 0.5)

        node_color_list = [node_colors.get(node, 'black') for node in G.nodes()]
        max_len = max(len(node) for node in G.nodes()) if G.nodes() else 10
        node_size = node_cfg.rb_node_size

        nx.draw_networkx_nodes(G, pos, node_color=node_color_list, node_size=node_size, ax=ax)
        nx.draw_networkx_edges(G, pos, arrows=True, arrowstyle='->', connectionstyle='arc3,rad=0.0', ax=ax)
        labels = {node: node for node in G.nodes()}
        nx.draw_networkx_labels(G, pos, labels, font_color='white', font_size=label_cfg.label_font_size, font_weight=label_cfg.label_font_weight, ax=ax)
        ax.set_title(f'RBTree Growth - Step {step}')

    ani = animation.FuncAnimation(fig, update, frames=len(snapshots), interval=2000, repeat=False)
    ani.save(output_file, writer='pillow')
    plt.close()
    print(f"Animation saved to {output_file}")

if __name__ == "__main__":
    snapshots_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else 'assets/gif/rbtree_growth.gif'
    snapshots = read_snapshots(snapshots_file)
    animate_rbtree_growth(snapshots, output_file)
