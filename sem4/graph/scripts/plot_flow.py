import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
from config import *
from utils import *


def main():
    input_file, output_file, graph_type, title = parse_args(
        'assets/txt/flow.txt',
        'assets/png/flow.png',
        'undirected'
    )

    G = create_graph(graph_type)
    read_flow_file(input_file, G)

    setup_figure()
    pos = get_pos(G)

    edges = list(G.edges())
    flows = [G[u][v]['flow'] for u, v in edges]
    capacities = [G[u][v]['capacity'] for u, v in edges]
    utilization = [
        (G[u][v]['flow'] / G[u][v]['capacity']) if G[u][v]['capacity'] > 0 else 0
        for u, v in edges
    ]

    draw_edges(
        G, pos, graph_type,
        width=[1.5 + f / 2 for f in flows],
        edge_color=utilization,
        edge_cmap=mpl.colormaps[FLOW_COLORMAP],
        edge_vmin=0,
        edge_vmax=1,
        alpha=FLOW_EDGE_ALPHA
    )

    draw_nodes(G, pos, FLOW_NODE_COLOR)
    draw_labels(G, pos)

    edge_labels = {
        (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
        for u, v in edges
    }
    draw_edge_labels(G, pos, edge_labels)

    legend_elements = [
        Line2D([0], [0], marker=LEGEND_NODE_MARKER, color='w', markerfacecolor=FLOW_NODE_COLOR, markersize=LEGEND_NODE_SIZE, label='Узлы сети'),
        Line2D([0], [0], color='blue', linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Низкая загрузка'),
        Line2D([0], [0], color='red', linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Высокая загрузка'),
        Line2D([0], [0], color='gray', linewidth=1, linestyle=LEGEND_EDGE_STYLE, label='Толщина = поток')
    ]
    plt.legend(handles=legend_elements, loc='upper left', fontsize=FONTSIZE)

    if title is None:
        title = f'Сеть потоков: толщина = поток, цвет = загрузка ({graph_type})'
    finalize_plot(output_file, title)


if __name__ == '__main__':
    main()
