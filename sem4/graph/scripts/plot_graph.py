#!/usr/bin/env python3
"""Визуализация графа с весами"""
import sys
import matplotlib as mpl
from matplotlib.lines import Line2D

from graph_loader import GraphLoader
from renderer import Renderer
from config import node_cfg, edge_cfg, colormap_cfg, legend_cfg


def main():
    graph_file = sys.argv[1]
    output_file = sys.argv[2]
    directed = len(sys.argv) > 3 and sys.argv[3].lower() == 'directed'

    loader = GraphLoader()
    G = loader.load_graph(graph_file, directed)

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    weights = [G[u][v]['weight'] for u, v in G.edges()]

    renderer.draw_edges(
        G, pos, directed,
        width=[edge_cfg.graph_edge_base + w * edge_cfg.graph_weight_multiplier for w in weights],
        edge_color=weights,
        edge_cmap=mpl.colormaps[colormap_cfg.graph_colormap],
        alpha=edge_cfg.default_edge_alpha
    )

    renderer.draw_nodes(G, pos)
    renderer.draw_labels(G, pos)

    edge_labels = {(u, v): f"{G[u][v]['weight']:.2f}" for u, v in G.edges()}
    renderer.draw_edge_labels(G, pos, edge_labels)

    legend = [
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.node_color, markersize=legend_cfg.legend_node_size,
               label='Узлы графа'),
        Line2D([0], [0], color='gray', linewidth=2, linestyle=legend_cfg.legend_edge_style,
               label='Рёбра (цвет зависит от веса)')
    ]
    renderer.add_legend(legend)

    title = f'Граф ({"directed" if directed else "undirected"})'
    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
