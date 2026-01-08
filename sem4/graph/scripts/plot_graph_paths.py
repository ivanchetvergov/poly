import sys
from matplotlib.lines import Line2D
import matplotlib.cm as cm

from graph_loader import GraphLoader
from renderer import Renderer
from config import node_cfg, edge_cfg, legend_cfg
from helpers import get_path_edges, normalize_edge

def read_paths(filename):
    with open(filename, 'r') as f:
        num_paths = int(f.readline().strip())
        paths = []
        for _ in range(num_paths):
            path = [int(x) for x in f.readline().strip().split()]
            paths.append(path)
        return paths

def main():
    graph_file = sys.argv[1]
    paths_file = sys.argv[2]
    output_file = sys.argv[3]
    directed = len(sys.argv) > 4 and sys.argv[4].lower() == 'directed'
    title = sys.argv[5].strip('"') if len(sys.argv) > 5 else f'Пути в графе ({"directed" if directed else "undirected"})'
    added_edges_file = 'assets/txt/added_edges.txt'

    loader = GraphLoader()
    G = loader.load_graph(graph_file, directed)
    paths = read_paths(paths_file)
    added_edges = loader.load_added_edges(added_edges_file)

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    if len(paths) == 1:
        colors = [edge_cfg.path_edge_color]
    else:
        colors = cm.rainbow([i / len(paths) for i in range(len(paths))])

    all_path_edges = {}
    for idx, path in enumerate(paths):
        path_edges = get_path_edges(path, directed)
        for edge in path_edges:
            if edge not in all_path_edges:
                all_path_edges[edge] = []
            all_path_edges[edge].append((idx, colors[idx]))

    edge_colors = []
    edge_widths = []
    for u, v in G.edges():
        edge = normalize_edge(u, v, directed)
        if edge in added_edges or (v, u) in added_edges:
            edge_colors.append(edge_cfg.added_edge_color)
            edge_widths.append(edge_cfg.added_edge_width)
        elif edge in all_path_edges:
            edge_colors.append(all_path_edges[edge][0][1])
            edge_widths.append(edge_cfg.path_edge_width if len(paths) == 1 else 3.0)
        else:
            edge_colors.append(edge_cfg.default_edge_color)
            edge_widths.append(edge_cfg.default_edge_width)

    renderer.draw_edges(G, pos, directed, edge_color=edge_colors, width=edge_widths,
                       alpha=edge_cfg.path_edge_alpha)

    all_path_nodes = set()
    for path in paths:
        all_path_nodes.update(path)

    node_colors = [
        node_cfg.path_node_color if node in all_path_nodes else node_cfg.node_color
        for node in G.nodes()
    ]
    renderer.draw_nodes(G, pos, node_colors)
    renderer.draw_labels(G, pos)

    edge_labels = {}
    for u, v in G.edges():
        edge = normalize_edge(u, v, directed)
        if len(paths) > 1 and edge in all_path_edges:
            path_nums = [str(idx + 1) for idx, _ in all_path_edges[edge]]
            edge_labels[(u, v)] = f"{G[u][v]['weight']:.2f}\n#{','.join(path_nums)}"
        else:
            edge_labels[(u, v)] = f"{G[u][v]['weight']:.2f}"
    renderer.draw_edge_labels(G, pos, edge_labels)

    legend = [
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.node_color, markersize=legend_cfg.legend_node_size,
               label='Обычные узлы'),
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.path_node_color, markersize=legend_cfg.legend_node_size,
               label='Узлы в путях')
    ]

    if len(paths) == 1:
        legend.append(
            Line2D([0], [0], color=colors[0], linewidth=edge_cfg.path_edge_width,
                   linestyle=legend_cfg.legend_edge_style, label='Путь')
        )
    else:
        for idx, color in enumerate(colors):
            legend.append(
                Line2D([0], [0], color=color, linewidth=3,
                       linestyle=legend_cfg.legend_edge_style, label=f'Путь #{idx + 1}')
            )

    if added_edges:
        legend.append(
            Line2D([0], [0], color=edge_cfg.added_edge_color, linewidth=edge_cfg.added_edge_width,
                   linestyle=legend_cfg.legend_edge_style, label='Добавленные рёбра')
        )

    renderer.add_legend(legend)

    full_title = title if len(paths) == 1 else f'{title} - найдено {len(paths)}'
    renderer.finalize(output_file, full_title)


if __name__ == '__main__':
    main()
