import matplotlib.pyplot as plt
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
from config import *
from utils import *

def main():
    input_file, output_file, graph_type, title, added_edges_file = parse_args_with_added_edges(
        'assets/txt/graph.txt',
        'assets/png/path.png',
        'undirected'
    )

    G = create_graph(graph_type)
    read_graph_file(input_file, G)

    path = read_path_file()
    added_edges = read_added_edges_file(added_edges_file) if added_edges_file else set()

    if G.number_of_nodes() == 0:
        print("[WARN] Граф пустой")
        return

    setup_figure()
    pos = get_pos(G)

    path_edges = get_path_edges(path, graph_type)

    edge_colors = []
    edge_widths = []
    for u, v in G.edges():
        edge = normalize_edge(u, v, graph_type)
        if edge in added_edges or (v, u) in added_edges:
            edge_colors.append(ADDED_EDGE_COLOR)
            edge_widths.append(ADDED_EDGE_WIDTH)
        elif edge in path_edges:
            edge_colors.append(PATH_EDGE_COLOR)
            edge_widths.append(PATH_EDGE_WIDTH)
        else:
            edge_colors.append(DEFAULT_EDGE_COLOR)
            edge_widths.append(DEFAULT_EDGE_WIDTH)

    draw_edges(
        G, pos, graph_type,
        edge_color=edge_colors,
        width=edge_widths,
        alpha=PATH_EDGE_ALPHA
    )

    node_colors = [PATH_NODE_COLOR if node in path else NODE_COLOR for node in G.nodes()]

    draw_nodes(G, pos, node_colors)
    draw_labels(G, pos)

    edge_labels = {k: f"{v:.2f}" for k, v in nx.get_edge_attributes(G, 'weight').items()}
    draw_edge_labels(G, pos, edge_labels)

    legend_elements = [
        Line2D([0], [0], marker=LEGEND_NODE_MARKER, color='w', markerfacecolor=NODE_COLOR, markersize=LEGEND_NODE_SIZE, label='Обычные узлы'),
        Line2D([0], [0], marker=LEGEND_NODE_MARKER, color='w', markerfacecolor=PATH_NODE_COLOR, markersize=LEGEND_NODE_SIZE, label='Узлы в пути'),
        Line2D([0], [0], color=DEFAULT_EDGE_COLOR, linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Обычные рёбра'),
        Line2D([0], [0], color=PATH_EDGE_COLOR, linewidth=2, linestyle=LEGEND_EDGE_STYLE, label='Рёбра в пути')
    ]

    if added_edges:
        legend_elements.append(Line2D([0], [0], color=ADDED_EDGE_COLOR, linestyle=LEGEND_EDGE_STYLE, label='Добавленные рёбра'))

    plt.legend(handles=legend_elements, loc='upper left', fontsize=FONTSIZE)

    if title is None:
        title = f'Граф с путём ({graph_type})'
    finalize_plot(output_file, title)


if __name__ == '__main__':
    main()
