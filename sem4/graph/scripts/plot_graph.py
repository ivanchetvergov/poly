import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
from config import *
from utils import *


def main():
    input_file, output_file, graph_type, title = parse_args(
        'assets/txt/graph.txt',
        'assets/png/graph.png'
    )

    G = create_graph(graph_type)
    read_graph_file(input_file, G)

    setup_figure()
    pos = get_pos(G)

    weights = [G[u][v]['weight'] for u, v in G.edges()]

    draw_edges(
        G, pos, graph_type,
        width=[0.7 + w / 3 for w in weights],
        edge_color=weights,
        edge_cmap=mpl.colormaps[GRAPH_COLORMAP],
        alpha=DEFAULT_EDGE_ALPHA
    )

    draw_nodes(G, pos)
    draw_labels(G, pos)

    edge_labels = {k: f"{v:.2f}" for k, v in nx.get_edge_attributes(G, 'weight').items()}
    draw_edge_labels(G, pos, edge_labels)

    legend_elements = [
        Line2D([0], [0], marker=LEGEND_NODE_MARKER, color='w', markerfacecolor=NODE_COLOR, markersize=LEGEND_NODE_SIZE, label='Узлы графа'),
        Line2D([0], [0], color='gray', linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Рёбра (цвет зависит от веса)')
    ]
    plt.legend(handles=legend_elements, loc='upper left', fontsize=FONTSIZE)

    if title is None:
        title = f'Граф ({graph_type})'
    finalize_plot(output_file, title)


if __name__ == '__main__':
    main()
