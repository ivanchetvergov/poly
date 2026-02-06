import sys
import argparse
from matplotlib.lines import Line2D
import matplotlib.cm as cm
import matplotlib as mpl
from typing import List, Tuple, Set, Dict

from ..core.graph_loader import GraphLoader
from ..core.renderer import Renderer
from ..core.config import node_cfg, edge_cfg, colormap_cfg, legend_cfg
from ..core.helpers import get_path_edges, normalize_edge, read_paths


def get_edge_colors_and_widths(G, directed, paths, added_edges, all_path_edges, colors, network_type):
    edge_colors = []
    edge_widths = []

    for u, v in G.edges():
        edge = normalize_edge(u, v, directed)
        if edge in added_edges or (v, u) in added_edges:
            edge_colors.append(edge_cfg.highlight_edge_color)
            edge_widths.append(edge_cfg.added_edge_width * edge_cfg.highlight_edge_width_multiplier)
        elif edge in all_path_edges:
            if network_type == 'flow':
                edge_colors.append('#00FF00')
                flow = G[u][v]['flow']
                edge_widths.append(3.0 + flow * edge_cfg.flow_multiplier)
            else:
                edge_colors.append(all_path_edges[edge][0][1])
                width = edge_cfg.path_edge_width if len(paths) == 1 else edge_cfg.path_edge_width * 0.7
                edge_widths.append(width)
        else:
            if network_type == 'flow':
                flow = G[u][v]['flow']
                capacity = G[u][v]['capacity']
                utilization = (flow / capacity) if capacity > 0 else 0
                cmap = mpl.colormaps[colormap_cfg.flow_colormap]
                edge_colors.append(cmap(utilization))
                edge_widths.append(edge_cfg.edge_width_base + flow * edge_cfg.flow_multiplier)
            else:
                edge_colors.append(edge_cfg.background_edge_color)
                edge_widths.append(edge_cfg.default_edge_width * edge_cfg.background_edge_width_multiplier)

    return edge_colors, edge_widths


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file')
    parser.add_argument('paths_file')
    parser.add_argument('output_file')
    parser.add_argument('directed', choices=['directed', 'undirected'])
    parser.add_argument('title', nargs='?', default='')
    parser.add_argument('--type', choices=['graph', 'flow'], default='graph')
    parser.add_argument('--added_edges_file', default='assets/txt/added_edges.txt')
    args = parser.parse_args()

    directed = args.directed == 'directed'
    network_type = args.type
    title = args.title.strip('"') if args.title else ''

    loader = GraphLoader()
    if network_type == 'graph':
        G = loader.load_graph(args.graph_file, directed)
        paths: List[List[int]] = read_paths(args.paths_file)
        added_edges: Set[Tuple[int, int]] = loader.load_added_edges(args.added_edges_file)
        default_title = f'Пути в графе ({"directed" if directed else "undirected"})'
        graph_title = title if title else default_title
    elif network_type == 'flow':
        G = loader.load_flow_network(args.graph_file, directed)
        path = loader.load_path(args.paths_file)
        paths = [path]
        added_edges = set()
        default_title = f'Сеть потоков с путём ({"directed" if directed else "undirected"})'
        graph_title = title if title else default_title
    else:
        raise ValueError(f"Unknown network type: {network_type}")

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    if not paths or all(len(p) == 0 for p in paths):
        colors = []
    elif len(paths) == 1:
        colors = [edge_cfg.path_edge_color] if network_type == 'graph' else ['#00FF00']
    else:
        colors = cm.rainbow([i / len(paths) for i in range(len(paths))])

    all_path_edges: Dict[Tuple[int, int], List[Tuple[int, any]]] = {}
    for idx, path in enumerate(paths):
        path_edges = get_path_edges(path, directed)
        for edge in path_edges:
            if edge not in all_path_edges:
                all_path_edges[edge] = []
            all_path_edges[edge].append((idx, colors[idx]))

    edge_colors, edge_widths = get_edge_colors_and_widths(
        G, directed, paths, added_edges, all_path_edges, colors, network_type
    )

    renderer.draw_edges(G, pos, directed, edge_color=edge_colors, width=edge_widths,
                       alpha=edge_cfg.background_edge_alpha if network_type == 'graph' else edge_cfg.flow_edge_alpha)

    all_path_nodes = set()
    for path in paths:
        all_path_nodes.update(path)

    if network_type == 'flow':
        node_colors = [
            node_cfg.path_node_color if node in all_path_nodes else node_cfg.flow_node_color
            for node in G.nodes()
        ]
    else:
        node_colors = [
            node_cfg.path_node_color if node in all_path_nodes else node_cfg.node_color
            for node in G.nodes()
        ]
    renderer.draw_nodes(G, pos, node_colors)
    renderer.draw_labels(G, pos)

    if network_type == 'flow':
        edge_labels = {
            (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
            for u, v in G.edges()
        }
    else:
        edge_labels = {}
        for u, v in G.edges():
            edge = normalize_edge(u, v, directed)
            if len(paths) > 1 and edge in all_path_edges:
                path_nums = [str(idx + 1) for idx, _ in all_path_edges[edge]]
                edge_labels[(u, v)] = f"{G[u][v]['weight']:.2f}\n#{','.join(path_nums)}"
            else:
                edge_labels[(u, v)] = f"{G[u][v]['weight']:.2f}"
    renderer.draw_edge_labels(G, pos, edge_labels)

    if network_type == 'flow':
        legend = [
            Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                   markerfacecolor=node_cfg.flow_node_color, markersize=legend_cfg.legend_node_size,
                   label='Обычные узлы'),
            Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                   markerfacecolor=node_cfg.path_node_color, markersize=legend_cfg.legend_node_size,
                   label='Узлы в пути'),
            Line2D([0], [0], color='blue', linewidth=2, linestyle=legend_cfg.legend_edge_style,
                   label='Низкая загрузка'),
            Line2D([0], [0], color='red', linewidth=2, linestyle=legend_cfg.legend_edge_style,
                   label='Высокая загрузка'),
            Line2D([0], [0], color='#00FF00', linewidth=3, linestyle=legend_cfg.legend_edge_style,
                   label='Рёбра в пути')
        ]
    else:
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

    full_title = graph_title if len(paths) == 1 or network_type == 'flow' else f'{graph_title} - найдено {len(paths)}'
    renderer.finalize(args.output_file, full_title)


if __name__ == '__main__':
    main()
