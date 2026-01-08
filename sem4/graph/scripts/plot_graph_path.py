
import sys
from matplotlib.lines import Line2D

from graph_loader import GraphLoader
from renderer import Renderer
from config import node_cfg, edge_cfg, legend_cfg
from helpers import get_path_edges, normalize_edge

def main():
    graph_file = sys.argv[1]
    path_file = sys.argv[2]
    output_file = sys.argv[3]
    directed = len(sys.argv) > 4 and sys.argv[4].lower() == 'directed'

    loader = GraphLoader()
    G = loader.load_graph(graph_file, directed)
    path = loader.load_path(path_file)
    added_edges = loader.load_added_edges('assets/txt/added_edges.txt')

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    path_edges = get_path_edges(path, directed)

    edge_colors = []
    edge_widths = []
    for u, v in G.edges():
        edge = normalize_edge(u, v, directed)
        if edge in added_edges or (v, u) in added_edges:
            edge_colors.append(edge_cfg.added_edge_color)
            edge_widths.append(edge_cfg.added_edge_width)
        elif edge in path_edges:
            edge_colors.append(edge_cfg.path_edge_color)
            edge_widths.append(edge_cfg.path_edge_width)
        else:
            edge_colors.append(edge_cfg.default_edge_color)
            edge_widths.append(edge_cfg.default_edge_width)

    renderer.draw_edges(G, pos, directed, edge_color=edge_colors, width=edge_widths,
                       alpha=edge_cfg.path_edge_alpha)

    node_colors = [
        node_cfg.path_node_color if node in path else node_cfg.node_color
        for node in G.nodes()
    ]
    renderer.draw_nodes(G, pos, node_colors)
    renderer.draw_labels(G, pos)

    edge_labels = {(u, v): f"{G[u][v]['weight']:.2f}" for u, v in G.edges()}
    renderer.draw_edge_labels(G, pos, edge_labels)

    legend = [
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.node_color, markersize=legend_cfg.legend_node_size,
               label='Обычные узлы'),
        Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
               markerfacecolor=node_cfg.path_node_color, markersize=legend_cfg.legend_node_size,
               label='Узлы в пути'),
        Line2D([0], [0], color=edge_cfg.default_edge_color, linewidth=2,
               linestyle=legend_cfg.legend_edge_style, label='Обычные рёбра'),
        Line2D([0], [0], color=edge_cfg.path_edge_color, linewidth=2,
               linestyle=legend_cfg.legend_edge_style, label='Рёбра в пути')
    ]
    if added_edges:
        legend.append(
            Line2D([0], [0], color=edge_cfg.added_edge_color,
                  linestyle=legend_cfg.legend_edge_style, label='Добавленные рёбра')
        )
    renderer.add_legend(legend)

    title = f'Граф с путём ({"directed" if directed else "undirected"})'
    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
