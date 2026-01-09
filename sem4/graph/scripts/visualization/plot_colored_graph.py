import sys
import matplotlib as mpl
from matplotlib.lines import Line2D

from ..core.graph_loader import GraphLoader
from ..core.renderer import Renderer
from ..core.config import node_cfg, edge_cfg, colormap_cfg, legend_cfg


def get_vertex_colors(G, colors_dict: dict) -> list:
    max_color = max(colors_dict.values()) if colors_dict else 0
    if max_color == 0:
        return [node_cfg.default_node_color for _ in G.nodes()]
    elif max_color == 1:
        return [node_cfg.highlight_node_color if colors_dict.get(node, 0) > 0 else node_cfg.default_node_color for node in G.nodes()]
    else:
        cmap = mpl.colormaps['Set1']
        return [cmap(colors_dict.get(node, 0) / (max_color + 1)) for node in G.nodes()]


def main():
    graph_file = sys.argv[1]
    output_file = sys.argv[2]
    colors_file = sys.argv[3]
    directed = len(sys.argv) > 4 and sys.argv[4].lower() == 'directed'
    title_arg = sys.argv[5] if len(sys.argv) > 5 else None

    loader = GraphLoader()
    G = loader.load_graph(graph_file, directed)
    colors = loader.load_colors(colors_file)

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    node_color_list = get_vertex_colors(G, colors)

    edge_color_list = []
    for u, v in G.edges():
        if colors.get(u, 0) > 0 or colors.get(v, 0) > 0:
            edge_color_list.append(edge_cfg.highlight_edge_color)
        else:
            edge_color_list.append(edge_cfg.default_edge_color)

    unique_colors = sorted(set(colors.values()))
    max_color = max(colors.values()) if colors else 0
    legend_elements = []
    if max_color == 1:
        legend_elements.append(Line2D([0], [0], marker='o', color='w',
                                     markerfacecolor=node_cfg.highlight_node_color, markersize=legend_cfg.legend_node_size,
                                     label='Выделенные вершины'))
        legend_elements.append(Line2D([0], [0], marker='o', color='w',
                                     markerfacecolor=node_cfg.default_node_color, markersize=legend_cfg.legend_node_size,
                                     label='Остальные вершины'))
    else:
        cmap = mpl.colormaps['Set1']
        for color_idx in unique_colors:
            color = cmap(color_idx / (max_color + 1)) if max_color > 0 else node_cfg.default_node_color
            legend_elements.append(Line2D([0], [0], marker='o', color='w',
                                         markerfacecolor=color, markersize=legend_cfg.legend_node_size,
                                         label=f'Цвет {color_idx + 1}'))

    legend_elements.append(Line2D([0], [0], color=edge_cfg.highlight_edge_color, linewidth=2,
                                 linestyle=legend_cfg.legend_edge_style, label='Рёбра выделенных вершин'))
    legend_elements.append(Line2D([0], [0], color=edge_cfg.default_edge_color, linewidth=2,
                                 linestyle=legend_cfg.legend_edge_style, label='Остальные рёбра'))

    weights = [G[u][v]['weight'] for u, v in G.edges()]

    renderer.draw_edges(
        G, pos, directed,
        width=[edge_cfg.graph_edge_base + w * edge_cfg.graph_weight_multiplier for w in weights],
        edge_color=edge_color_list,
        alpha=edge_cfg.default_edge_alpha
    )

    renderer.draw_nodes(G, pos, node_colors=node_color_list)
    renderer.draw_labels(G, pos)

    edge_labels = {(u, v): f"{G[u][v]['weight']:.2f}" for u, v in G.edges()}
    renderer.draw_edge_labels(G, pos, edge_labels)

    renderer.add_legend(legend_elements)

    title = title_arg.strip('"') if title_arg else f'Раскраска графа ({"directed" if directed else "undirected"})'
    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
