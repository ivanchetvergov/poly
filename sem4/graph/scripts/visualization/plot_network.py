import sys
import argparse
import matplotlib as mpl
from matplotlib.lines import Line2D

from ..core.graph_loader import GraphLoader
from ..core.renderer import Renderer
from ..core.config import node_cfg, edge_cfg, colormap_cfg, legend_cfg
from ..core.helpers import normalize_edge


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file')
    parser.add_argument('output_file')
    parser.add_argument('directed', choices=['directed', 'undirected'])
    parser.add_argument('title', nargs='?', default='')
    parser.add_argument('--type', choices=['graph', 'flow'], default='graph')
    parser.add_argument('--added_edges_file', default='')
    args = parser.parse_args()

    directed = args.directed == 'directed'
    network_type = args.type
    title = args.title.strip('"') if args.title else ''

    loader = GraphLoader()
    if network_type == 'graph':
        G = loader.load_graph(args.graph_file, directed)
        added_edges = loader.load_added_edges(args.added_edges_file) if args.added_edges_file else set()
        default_title = f'Граф ({"directed" if directed else "undirected"})'
        graph_title = title if title else default_title
    elif network_type == 'flow':
        G = loader.load_flow_network(args.graph_file, directed)
        added_edges = set()
        default_title = f'Сеть потоков: толщина = поток, цвет = загрузка ({"directed" if directed else "undirected"})'
        graph_title = title if title else default_title
    else:
        raise ValueError(f"Unknown network type: {network_type}")

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    try:
        import json
        pos_file = args.graph_file + '.pos.json'
        serializable = {str(n): [float(x), float(y)] for n, (x, y) in pos.items()}
        with open(pos_file, 'w', encoding='utf-8') as pf:
            json.dump(serializable, pf)
    except Exception:
        pass

    if network_type == 'graph':
        weights = [G[u][v]['weight'] for u, v in G.edges()]
        min_w = min(weights) if weights else 0
        max_w = max(weights) if weights else 1
        if max_w == min_w:
            norm_weights = [0.5 for _ in weights]
        else:
            norm_weights = [(w - min_w) / (max_w - min_w) for w in weights]
        cmap = mpl.colormaps[colormap_cfg.graph_colormap]
        edge_colors = []
        edge_widths = []
        for i, (u, v) in enumerate(G.edges()):
            edge = normalize_edge(u, v, directed)
            if edge in added_edges or (v, u) in added_edges:
                edge_colors.append(edge_cfg.highlight_edge_color)
                edge_widths.append(edge_cfg.added_edge_width)
            else:
                edge_colors.append(cmap(norm_weights[i]))
                edge_widths.append(edge_cfg.graph_edge_base + weights[i] * edge_cfg.graph_weight_multiplier)
        renderer.draw_edges(
            G, pos, directed,
            width=edge_widths,
            edge_color=edge_colors,
            alpha=edge_cfg.default_edge_alpha
        )
        renderer.draw_nodes(G, pos)
        edge_labels = {(u, v): f"{G[u][v]['weight']:.2f}" for u, v in G.edges()}
        legend = [
            Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                   markerfacecolor=node_cfg.node_color, markersize=legend_cfg.legend_node_size,
                   label='Узлы графа'),
            Line2D([0], [0], color='gray', linewidth=2, linestyle=legend_cfg.legend_edge_style,
                   label='Рёбра (цвет зависит от веса)')
        ]
        if added_edges:
            legend.append(
                Line2D([0], [0], color=edge_cfg.highlight_edge_color, linewidth=edge_cfg.added_edge_width,
                       linestyle=legend_cfg.legend_edge_style, label='Выделенные рёбра')
            )
    else:  # flow
        edges = list(G.edges())
        flows = [G[u][v]['flow'] for u, v in edges]
        utilization = [
            (G[u][v]['flow'] / G[u][v]['capacity']) if G[u][v]['capacity'] > 0 else 0
            for u, v in edges
        ]
        renderer.draw_edges(
            G, pos, directed,
            width=[edge_cfg.edge_width_base + f * edge_cfg.flow_multiplier for f in flows],
            edge_color=utilization,
            edge_cmap=mpl.colormaps[colormap_cfg.flow_colormap],
            edge_vmin=0,
            edge_vmax=1,
            alpha=edge_cfg.flow_edge_alpha
        )
        renderer.draw_nodes(G, pos, node_cfg.flow_node_color)
        edge_labels = {
            (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
            for u, v in edges
        }
        legend = [
            Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                   markerfacecolor=node_cfg.flow_node_color, markersize=legend_cfg.legend_node_size,
                   label='Узлы сети'),
            Line2D([0], [0], color='blue', linewidth=2, linestyle=legend_cfg.legend_edge_style,
                   label='Низкая загрузка'),
            Line2D([0], [0], color='red', linewidth=2, linestyle=legend_cfg.legend_edge_style,
                   label='Высокая загрузка'),
            Line2D([0], [0], color='gray', linewidth=1, linestyle=legend_cfg.legend_edge_style,
                   label='Толщина = поток')
        ]

    renderer.draw_labels(G, pos)
    renderer.draw_edge_labels(G, pos, edge_labels)
    renderer.add_legend(legend)
    renderer.finalize(args.output_file, graph_title)


if __name__ == '__main__':
    main()
