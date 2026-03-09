import sys
import argparse
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.lines import Line2D
import networkx as nx

from ..core.graph_loader import GraphLoader
from ..core.renderer import Renderer
from ..core.config import node_cfg, edge_cfg, legend_cfg, plot_cfg


def read_bfs_edges(filename: str) -> list[tuple[int, int]]:
    edges = []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 2:
                edges.append((int(parts[0]), int(parts[1])))
    return edges


def animate_bfs(graph_file: str, bfs_edges_file: str, output_file: str,
                directed: bool = False) -> None:
    loader = GraphLoader()
    G = loader.load_graph(graph_file, directed)
    bfs_edges = read_bfs_edges(bfs_edges_file)

    renderer = Renderer()
    renderer.setup_plot()
    pos = nx.spring_layout(G, seed=plot_cfg.seed, k=1.5, iterations=50)

    all_edges = list(G.edges())
    all_nodes = list(G.nodes())

    # frame 0  = base graph only; frames 1..N = one edge revealed per frame
    num_frames = len(bfs_edges) + 1

    def update(frame: int) -> None:
        renderer.ax.clear()

        revealed_edges: set[tuple[int, int]] = set(bfs_edges[:frame])
        visited_nodes: set[int] = set()
        if bfs_edges:
            start_node = bfs_edges[0][0]
            visited_nodes.add(start_node)
        for u, v in bfs_edges[:frame]:
            visited_nodes.add(u)
            visited_nodes.add(v)

        # background edges
        bg_colors = []
        bg_widths = []
        for u, v in all_edges:
            if (u, v) in revealed_edges or (v, u) in revealed_edges:
                bg_colors.append(edge_cfg.highlight_edge_color)
                bg_widths.append(edge_cfg.added_edge_width)
            else:
                bg_colors.append(edge_cfg.background_edge_color)
                bg_widths.append(edge_cfg.default_edge_width * edge_cfg.background_edge_width_multiplier)

        renderer.draw_edges(
            G, pos, directed,
            width=bg_widths,
            edge_color=bg_colors,
            alpha=edge_cfg.default_edge_alpha,
        )

        # nodes
        node_colors = []
        for n in all_nodes:
            if n in visited_nodes:
                node_colors.append(node_cfg.path_node_color)
            else:
                node_colors.append(node_cfg.node_color)

        renderer.draw_nodes(G, pos, node_colors)
        renderer.draw_labels(G, pos)

        edge_labels = {(u, v): f"{G[u][v]['weight']:.1f}" for u, v in all_edges}
        renderer.draw_edge_labels(G, pos, edge_labels)

        legend_elements = [
            Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                   markerfacecolor=node_cfg.path_node_color, markersize=legend_cfg.legend_node_size,
                   label='Посещённые вершины'),
            Line2D([0], [0], marker=legend_cfg.legend_node_marker, color='w',
                   markerfacecolor=node_cfg.node_color, markersize=legend_cfg.legend_node_size,
                   label='Непосещённые вершины'),
            Line2D([0], [0], color=edge_cfg.highlight_edge_color, linewidth=edge_cfg.added_edge_width,
                   label='Рёбра BFS-дерева'),
        ]
        renderer.ax.legend(handles=legend_elements, loc='best', fontsize=plot_cfg.fontsize)

        step_label = f"Шаг {frame} / {len(bfs_edges)}"
        if frame > 0:
            u, v = bfs_edges[frame - 1]
            step_label += f";  открыто ребро {u} -> {v}"
        renderer.ax.set_title(f"BFS-обход: {step_label}",
                               fontsize=plot_cfg.title_font_size,
                               fontweight=plot_cfg.title_font_weight)

    ani = animation.FuncAnimation(
        renderer.fig, update,
        frames=num_frames,
        interval=800,
        repeat=False,
    )
    ani.save(output_file, writer='pillow')
    plt.close()
    print(f"[OK] BFS-анимация сохранена в {output_file}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file')
    parser.add_argument('bfs_edges_file')
    parser.add_argument('output_file')
    parser.add_argument('directed', choices=['directed', 'undirected'])
    args = parser.parse_args()

    animate_bfs(args.graph_file, args.bfs_edges_file, args.output_file,
                directed=args.directed == 'directed')


if __name__ == '__main__':
    main()
