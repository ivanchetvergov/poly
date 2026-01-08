import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
from config import *
from utils import *


def main():
    input_file, output_file, graph_type, title = parse_args(
        'assets/txt/flow.txt',
        'assets/png/flow_path.png',
        'undirected'
    )

    G = create_graph(graph_type)
    read_flow_file(input_file, G)

    path = read_path_file()

    setup_figure()
    pos = get_pos(G)

    edges = list(G.edges())
    path_edges = get_path_edges(path, graph_type)

    edge_colors = []
    edge_widths = []

    for u, v in edges:
        edge = normalize_edge(u, v, graph_type)
        flow = G[u][v]['flow']
        capacity = G[u][v]['capacity']
        utilization = (flow / capacity) if capacity > 0 else 0

        if edge in path_edges:
            edge_colors.append('#00FF00')
            edge_widths.append(3.0 + flow / 2)
        else:
            cmap = mpl.colormaps[FLOW_COLORMAP]
            edge_colors.append(cmap(utilization))
            edge_widths.append(1.5 + flow / 2)

    draw_edges(
        G, pos, graph_type,
        width=edge_widths,
        edge_color=edge_colors,
        alpha=FLOW_EDGE_ALPHA
    )

    node_colors = [PATH_NODE_COLOR if node in path else FLOW_NODE_COLOR for node in G.nodes()]
    draw_nodes(G, pos, node_colors)
    draw_labels(G, pos)

    edge_labels = {
        (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
        for u, v in edges
    }
    draw_edge_labels(G, pos, edge_labels)

    legend_elements = [
        Line2D([0], [0], marker=LEGEND_NODE_MARKER, color='w', markerfacecolor=FLOW_NODE_COLOR, markersize=LEGEND_NODE_SIZE, label='Обычные узлы'),
        Line2D([0], [0], marker=LEGEND_NODE_MARKER, color='w', markerfacecolor=PATH_NODE_COLOR, markersize=LEGEND_NODE_SIZE, label='Узлы в пути'),
        Line2D([0], [0], color='blue', linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Низкая загрузка'),
        Line2D([0], [0], color='red', linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Высокая загрузка'),
        Line2D([0], [0], color='#00FF00', linewidth=3, linestyle=LEGEND_EDGE_STYLE, label='Рёбра в пути')
    ]
    plt.legend(handles=legend_elements, loc='upper left', fontsize=FONTSIZE)

    if title is None:
        title = f'Сеть потоков с путём ({graph_type})'
    finalize_plot(output_file, title)


if __name__ == '__main__':
    main()
