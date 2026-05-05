import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import networkx as nx

from scripts.core.renderer import Renderer
from scripts.core.config import plot_cfg, node_cfg, edge_cfg, label_cfg
from .plot_rbtree import build_rbtree_graph_from_text, hierarchy_pos, is_nil_node

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
    return build_rbtree_graph_from_text(data)

def animate_rbtree_growth(snapshots, output_file='assets/gif/rbtree_growth.gif'):
    if not snapshots:
        print("No snapshots to animate")
        return

    if len(snapshots) < 2:
        print(f"Warning: Only {len(snapshots)} snapshot(s), animation may not be useful")

    fig = plt.figure(figsize=(plot_cfg.figsize))
    ax = fig.add_axes([0, 0, 1, 1])

    def update(frame):
        ax.clear()
        step, data = snapshots[frame]
        G, node_colors, parents = create_graph_from_data(data)
        if not G.nodes():
            ax.set_title(f'RBTree Growth - Step {step} (empty)')
            return

        roots = [node for node, parent in parents.items() if parent == 'null']
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

        node_size = node_cfg.rb_node_size

        actual_nodes = [node for node in G.nodes() if not is_nil_node(node)]
        nil_nodes = [node for node in G.nodes() if is_nil_node(node)]

        actual_node_colors = [node_colors.get(node, 'black') for node in actual_nodes]
        if actual_nodes:
            nx.draw_networkx_nodes(G, pos, nodelist=actual_nodes, node_color=actual_node_colors, node_size=node_size, ax=ax)

        if nil_nodes:
            nil_size = max(80, int(node_size * 0.18))
            nx.draw_networkx_nodes(G, pos, nodelist=nil_nodes, node_color='black', node_size=nil_size, linewidths=0.5, edgecolors='gray', ax=ax)

        nx.draw_networkx_edges(G, pos, arrows=True, arrowstyle='->', connectionstyle='arc3,rad=0.0', ax=ax)
        labels = {node: node for node in actual_nodes}
        nx.draw_networkx_labels(G, pos, labels, font_color='white', font_size=label_cfg.label_font_size, font_weight=label_cfg.label_font_weight, ax=ax)
        ax.set_title(f'RBTree Growth - Step {step}', fontsize=14, fontweight='bold')
        ax.axis('off')

    # Use faster interval (500ms = 0.5 sec per frame, faster than 2000ms)
    ani = animation.FuncAnimation(fig, update, frames=len(snapshots), interval=500, repeat=True)

    try:
        ani.save(output_file, writer='pillow')
        print(f"Animation saved to {output_file}")
    except Exception as e:
        print(f"Error saving animation: {e}")

    plt.close()


if __name__ == "__main__":
    snapshots_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else 'assets/gif/rbtree_growth.gif'
    snapshots = read_snapshots(snapshots_file)
    animate_rbtree_growth(snapshots, output_file)
