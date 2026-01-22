import sys
import networkx as nx
from scripts.core.renderer import Renderer
from scripts.core.config import plot_cfg, node_cfg, edge_cfg, label_cfg

def main():
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "RBTree Visualization"

    G = nx.DiGraph()
    node_colors = []

    with open(data_file, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 3:
                continue
            node, parent, color = parts
            G.add_node(node)
            node_colors.append('red' if color == 'RED' else 'black')
            if parent != 'null':
                G.add_edge(parent, node)

    renderer = Renderer()
    renderer.setup_plot()
    pos = nx.spring_layout(G, seed=plot_cfg.seed)

    renderer.draw_nodes(G, pos, node_colors=node_colors)
    renderer.draw_edges(G, pos, directed=True)
    renderer.draw_labels(G, pos)

    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
