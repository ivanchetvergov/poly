
import sys
import matplotlib as mpl
from matplotlib.lines import Line2D

from ..core.graph_loader import GraphLoader
from ..core.renderer import Renderer
from ..core.config import node_cfg, edge_cfg, colormap_cfg, legend_cfg


def main():
    flow_file = sys.argv[1]
    output_file = sys.argv[2]
    directed = True if len(sys.argv) <= 3 else sys.argv[3].lower() == 'directed'
    title = ' '.join(sys.argv[4:]) if len(sys.argv) > 4 else f'Сеть потоков'

    loader = GraphLoader()
    G = loader.load_flow_network(flow_file, directed)

    renderer = Renderer()
    renderer.setup_plot()
    pos = renderer.compute_layout(G)

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
    renderer.draw_labels(G, pos)

    edge_labels = {
        (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
        for u, v in edges
    }
    renderer.draw_edge_labels(G, pos, edge_labels)

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
    renderer.add_legend(legend)

    renderer.finalize(output_file, title)


if __name__ == '__main__':
    main()
