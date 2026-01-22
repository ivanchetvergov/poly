import sys
import networkx as nx
from scripts.core.renderer import Renderer
from scripts.core.config import plot_cfg, node_cfg, edge_cfg, label_cfg

def main():
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "HashTable Visualization"

    G = nx.DiGraph()
    pos = {}
    bucket_colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']

    with open(data_file, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) < 3:
                continue
            bucket = int(parts[0])
            keys_counts = parts[1:]
            for i in range(0, len(keys_counts), 2):
                key = keys_counts[i]
                count = int(keys_counts[i+1])
                node = f"{key}({count})"
                G.add_node(node)
                pos[node] = (i, -bucket)
                if i > 0:
                    prev_node = f"{keys_counts[i-2]}({keys_counts[i-1]})"
                    G.add_edge(prev_node, node)

    renderer = Renderer()
    renderer.setup_plot()

    node_colors = [bucket_colors[bucket % len(bucket_colors)] for node in G.nodes() for bucket in range(10) if -pos[node][1] == bucket]

    renderer.draw_nodes(G, pos, node_colors=node_colors)
    renderer.draw_edges(G, pos, directed=True)
    renderer.draw_labels(G, pos)

    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
