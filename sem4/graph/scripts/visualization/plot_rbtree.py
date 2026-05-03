import sys
import networkx as nx
from scripts.core.renderer import Renderer
from scripts.core.config import plot_cfg, node_cfg, edge_cfg, label_cfg

def hierarchy_pos(G, root, width=1., vert_gap=0.2, vert_loc=1, xcenter=0.5):
    pos = {root: (xcenter, vert_loc)}
    neighbors = list(G.successors(root))
    if len(neighbors) != 0:
        dx = width / len(neighbors)
        nextx = xcenter - width/2 - dx/2
        for neighbor in neighbors:
            nextx += dx
            pos.update(hierarchy_pos(G, neighbor, width=dx, vert_gap=vert_gap, vert_loc=vert_loc-vert_gap, xcenter=nextx))
    return pos

def main():
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "RBTree Visualization"

    G = nx.DiGraph()
    node_colors = {}

    nodes = []
    edges = []
    with open(data_file, 'r', encoding='utf-8') as f:
        for line in f:
            raw = line.rstrip('\n')
            if not raw:
                continue

            if '\t' in raw:
                fields = raw.split('\t')
                if len(fields) != 3:
                    continue
                node, parent, color = fields
            else:
                parts = raw.split()
                if len(parts) < 3:
                    continue
                parent = parts[-2]
                color = parts[-1]
                node = ' '.join(parts[:-2])

            nodes.append((node, color))
            if parent != 'null':
                edges.append((parent, node))

    if not nodes:
        renderer = Renderer()
        renderer.setup_plot()
        renderer.finalize(output_file, title + ' (no data)')
        return

    for node, color in nodes:
        G.add_node(node)
        node_colors[node] = 'red' if color == 'RED' else 'black'

    for parent, node in edges:
        if parent in node_colors and node in node_colors:
            G.add_edge(parent, node)

    node_color_list = [node_colors.get(node, 'black') for node in G.nodes()]

    roots = [n for n in G.nodes() if G.in_degree(n) == 0]
    if not roots:
        roots = [next(iter(G.nodes()))]

    # Build layout for all roots/components so every node has a position.
    pos = {}
    span = 4.0
    for i, root in enumerate(roots):
        xcenter = i * (span + 1.0)
        pos.update(hierarchy_pos(G, root, width=span, vert_gap=0.5, vert_loc=1, xcenter=xcenter))

    # Safety fallback: place any still-unplaced nodes into a side column.
    missing = [n for n in G.nodes() if n not in pos]
    for i, node in enumerate(missing):
        pos[node] = (len(roots) * (span + 1.0) + 1.0, 1 - i * 0.5)

    renderer = Renderer()
    renderer.setup_plot()

    max_len = max(len(node) for node in G.nodes())
    node_size = node_cfg.rb_node_size
    nx.draw_networkx_nodes(G, pos, node_color=node_color_list, node_size=node_size)

    nx.draw_networkx_edges(G, pos, arrows=True, arrowstyle='->', connectionstyle='arc3,rad=0.0')

    labels = {node: node for node in G.nodes()}
    nx.draw_networkx_labels(G, pos, labels, font_color='white', font_size=label_cfg.label_font_size, font_weight=label_cfg.label_font_weight)

    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
