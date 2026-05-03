import sys
import os
import argparse
from matplotlib.lines import Line2D
import matplotlib.cm as cm
import matplotlib as mpl
import networkx as nx
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


def draw_euler_mode(renderer, G_undirected, pos, paths, added_edges, colors, graph_title, output_file):
    """
    Режим эйлерова цикла:
    - Рисуем исходный граф как undirected (серые рёбра, без стрелок)
    - Поверх строим DiGraph из последовательности пути и рисуем его directed (стрелки, цвет пути)
    Так каждое ребро — ровно одна стрелка в нужную сторону, никаких наложений.
    """
    path = paths[0] if paths else []

    # 1. Серые рёбра исходного графа без стрелок
    bg_colors = []
    bg_widths = []
    for u, v in G_undirected.edges():
        edge = (min(u, v), max(u, v))
        if edge in added_edges or (v, u) in added_edges:
            bg_colors.append(edge_cfg.highlight_edge_color)
            bg_widths.append(edge_cfg.added_edge_width)
        else:
            bg_colors.append(edge_cfg.background_edge_color)
            bg_widths.append(edge_cfg.default_edge_width * edge_cfg.background_edge_width_multiplier)

    renderer.draw_edges(
        G_undirected, pos,
        directed=False,
        edge_color=bg_colors,
        width=bg_widths,
        alpha=edge_cfg.background_edge_alpha
    )

    missing_added_edges = []
    for u, v in added_edges:
        if not G_undirected.has_edge(u, v):
            missing_added_edges.append((u, v))

    if missing_added_edges:
        G_added = nx.Graph()
        G_added.add_nodes_from(G_undirected.nodes())
        G_added.add_edges_from(missing_added_edges)
        renderer.draw_edges(
            G_added, pos,
            directed=False,
            edge_color=edge_cfg.highlight_edge_color,
            width=edge_cfg.added_edge_width,
            style='dashed',
            alpha=1.0
        )

    if len(path) >= 2:
        G_path = nx.DiGraph()
        G_path.add_nodes_from(G_undirected.nodes())
        for i in range(len(path) - 1):
            u, v = path[i], path[i + 1]
            w = G_undirected[u][v]['weight'] if G_undirected.has_edge(u, v) else \
                G_undirected[v][u]['weight'] if G_undirected.has_edge(v, u) else 1.0
            G_path.add_edge(u, v, weight=w)

        path_color = colors[0] if colors else edge_cfg.path_edge_color

        renderer.draw_edges(
            G_path, pos,
            directed=True,
            edge_color=path_color,
            width=edge_cfg.path_edge_width,
            alpha=1.0
        )

        edge_labels = {
            (u, v): f"{G_path[u][v]['weight']:.2f}"
            for u, v in G_path.edges()
        }
        renderer.draw_edge_labels(G_path, pos, edge_labels)

    start_node = path[0] if path else None
    path_nodes = set(path)
    node_colors = []
    for node in G_undirected.nodes():
        if node == start_node:
            node_colors.append(node_cfg.start_node_color)
        elif node in path_nodes:
            node_colors.append(node_cfg.path_node_color)
        else:
            node_colors.append(node_cfg.node_color)

    renderer.draw_nodes(G_undirected, pos, node_colors)
    renderer.draw_labels(G_undirected, pos)

    legend = [
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.node_color, markersize=legend_cfg.legend_node_size,
               label='Обычные узлы'),
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.path_node_color, markersize=legend_cfg.legend_node_size,
               label='Узлы в цикле'),
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.start_node_color, markersize=legend_cfg.legend_node_size,
               label='Начало / конец цикла'),
        Line2D([0], [0], color=edge_cfg.path_edge_color, linewidth=edge_cfg.path_edge_width,
               linestyle=legend_cfg.legend_edge_style, label='Эйлеров цикл'),
    ]
    if added_edges:
        legend.append(
            Line2D([0], [0], color=edge_cfg.highlight_edge_color, linewidth=edge_cfg.added_edge_width,
                   linestyle=legend_cfg.legend_edge_style, label='Добавленные рёбра')
        )
    if missing_added_edges:
        legend.append(
            Line2D([0], [0], color=edge_cfg.highlight_edge_color, linewidth=edge_cfg.added_edge_width,
                   linestyle='dashed', label='Достроенные (вне исходного графа)')
        )
    renderer.add_legend(legend)
    renderer.finalize(output_file, graph_title)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file')
    parser.add_argument('paths_file')
    parser.add_argument('output_file')
    parser.add_argument('directed', choices=['directed', 'undirected'])
    parser.add_argument('title', nargs='?', default='')
    parser.add_argument('--type', choices=['graph', 'flow'], default='graph')
    parser.add_argument('--added_edges_file', default='assets/txt/added_edges.txt')
    parser.add_argument('--euler', action='store_true', default=False)
    args = parser.parse_args()

    directed = args.directed == 'directed'
    network_type = args.type
    title = args.title.strip('"') if args.title else ''

    loader = GraphLoader()

    if args.euler:
        # Euler mode: всегда грузим как undirected
        G = loader.load_graph(args.graph_file, directed=False)
        paths: List[List[int]] = read_paths(args.paths_file)
        added_edges: Set[Tuple[int, int]] = loader.load_added_edges(args.added_edges_file)
        graph_title = title if title else 'Эйлеров цикл'

        renderer = Renderer()
        renderer.setup_plot()
        pos = renderer.compute_layout(G)

        colors = [edge_cfg.path_edge_color]
        draw_euler_mode(renderer, G, pos, paths, added_edges, colors, graph_title, args.output_file)
        return

    # ── Стандартный режим (не euler) ─────────────────────────────────────────
    if network_type == 'graph':
        G = loader.load_graph(args.graph_file, directed)
        paths = read_paths(args.paths_file)
        added_edges = loader.load_added_edges(args.added_edges_file)
        default_title = f'Пути в графе ({"directed" if directed else "undirected"})'
        graph_title = title if title else default_title
    elif network_type == 'flow':
        G = loader.load_flow_network(args.graph_file, directed)
        paths = read_paths(args.paths_file)
        if not paths:
            path = loader.load_path(args.paths_file)
            paths = [path] if path else []
        added_edges = set()
        default_title = f'Сеть потоков с путём ({"directed" if directed else "undirected"})'
        graph_title = title if title else default_title
    else:
        raise ValueError(f"Unknown network type: {network_type}")

    renderer = Renderer()
    renderer.setup_plot()

    pos = None
    try:
        import json
        pos_file = args.graph_file + '.pos.json'
        if os.path.exists(pos_file):
            with open(pos_file, 'r', encoding='utf-8') as pf:
                raw = json.load(pf)
            pos = {}
            for k, v in raw.items():
                try:
                    nk = int(k)
                except Exception:
                    nk = k
                pos[nk] = (float(v[0]), float(v[1]))
    except Exception:
        pos = None
    if pos is None:
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

    start_node = None
    end_node = None
    if len(paths) == 1 and paths[0]:
        start_node = paths[0][0]
        end_node = paths[0][-1]
        new_node_colors = []
        for node in G.nodes():
            if node == start_node:
                new_node_colors.append(node_cfg.start_node_color)
            elif node == end_node and end_node != start_node:
                new_node_colors.append(node_cfg.end_node_color)
            elif node in all_path_nodes:
                new_node_colors.append(node_cfg.path_node_color)
            else:
                new_node_colors.append(node_cfg.node_color)
        renderer.draw_nodes(G, pos, new_node_colors)

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
                Line2D([0], [0], color=edge_cfg.highlight_edge_color, linewidth=edge_cfg.added_edge_width,
                       linestyle=legend_cfg.legend_edge_style, label='Добавленные рёбра')
            )

    renderer.add_legend(legend)

    if start_node is not None:
        legend.append(Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                              markerfacecolor=node_cfg.start_node_color, markersize=legend_cfg.legend_node_size,
                              label='Start node'))
        if end_node is not None and end_node != start_node:
            legend.append(Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                                  markerfacecolor=node_cfg.end_node_color, markersize=legend_cfg.legend_node_size,
                                  label='End node'))
        renderer.add_legend(legend)

    full_title = (
        graph_title if len(paths) == 1 or network_type == 'flow'
        else f'{graph_title} - найдено {len(paths)}'
    )
    renderer.finalize(args.output_file, full_title)


if __name__ == '__main__':
    main()
