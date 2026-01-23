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

    with open(data_file, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 3:
                continue
            node, parent, color = parts
            G.add_node(node)
            node_colors[node] = 'red' if color == 'RED' else 'black'
            if parent != 'null' and parent in node_colors:
                G.add_edge(parent, node)

    node_color_list = [node_colors[node] for node in G.nodes()]

    root = [n for n in G.nodes() if G.in_degree(n) == 0][0]
    pos = hierarchy_pos(G, root, width=4.0, vert_gap=0.5, vert_loc=1)

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
