import sys
import matplotlib as mpl
from matplotlib.lines import Line2D

from ..core.graph_loader import GraphLoader
from ..core.renderer import Renderer
from ..core.config import node_cfg, edge_cfg, colormap_cfg, legend_cfg
from ..core.helpers import get_path_edges, normalize_edge

def main():
    flow_file = sys.argv[1]
    path_file = sys.argv[2]
    output_file = sys.argv[3]
    directed = True if len(sys.argv) <= 4 else sys.argv[4].lower() == 'directed'

    loader = GraphLoader()
    G = loader.load_flow_network(flow_file, directed)
    path = loader.load_path(path_file)

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

    path_edges = get_path_edges(path, directed)
    edges = list(G.edges())

    edge_colors = []
    edge_widths = []
    for u, v in edges:
        edge = normalize_edge(u, v, directed)
        flow = G[u][v]['flow']
        capacity = G[u][v]['capacity']
        utilization = (flow / capacity) if capacity > 0 else 0

        if edge in path_edges:
            edge_colors.append('#00FF00')
            edge_widths.append(3.0 + flow * edge_cfg.flow_multiplier)
        else:
            cmap = mpl.colormaps[colormap_cfg.flow_colormap]
            edge_colors.append(cmap(utilization))
            edge_widths.append(edge_cfg.edge_width_base + flow * edge_cfg.flow_multiplier)

    renderer.draw_edges(G, pos, directed, width=edge_widths, edge_color=edge_colors,
                       alpha=edge_cfg.flow_edge_alpha)

    node_colors = [
        node_cfg.path_node_color if node in path else node_cfg.flow_node_color
        for node in G.nodes()
    ]
    renderer.draw_nodes(G, pos, node_colors)
    renderer.draw_labels(G, pos)

    edge_labels = {
        (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
        for u, v in edges
    }
    renderer.draw_edge_labels(G, pos, edge_labels)

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
    renderer.add_legend(legend)

    title = f'Сеть потоков с путём ({"directed" if directed else "undirected"})'
    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
