#!/usr/bin/env python3
import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib as mpl
import networkx as nx
from matplotlib.lines import Line2D
import webbrowser
import os

from graph_loader import GraphLoader
from renderer import Renderer
from config import node_cfg, edge_cfg, colormap_cfg, legend_cfg, animation_cfg


def read_snapshots(filename='assets/txt/flow_snapshots.txt'):
    with open(filename, 'r') as f:
        num_snapshots = int(f.readline().strip())
        snapshots = []

        for _ in range(num_snapshots):
            step, total_flow = map(float, f.readline().strip().split())
            num_edges = int(f.readline().strip())

            edges = []
            for _ in range(num_edges):
                u, v, flow, capacity = f.readline().strip().split()
                edges.append((int(u), int(v), float(flow), float(capacity)))

            snapshots.append({
                'step': int(step),
                'total_flow': total_flow,
                'edges': edges
            })

        return snapshots


def create_graph_from_snapshot(snapshot):
    G = nx.DiGraph()
    for u, v, flow, capacity in snapshot['edges']:
        G.add_edge(u, v, flow=flow, capacity=capacity)
    return G


def animate_flow_growth(snapshots, output_file='assets/gif/flow_animation.gif',
                       directed=True, show=True):
    if not snapshots:
        print("Нет snapshots")
        return

    renderer = Renderer()
    renderer.setup_plot()
    G_initial = create_graph_from_snapshot(snapshots[0])
    pos = renderer.compute_layout(G_initial)

    def update(frame):
        renderer.ax.clear()
        snapshot = snapshots[frame]
        G = create_graph_from_snapshot(snapshot)

        edges = list(G.edges())
        utilization = []
        edge_widths = []

        for u, v in edges:
            flow = G[u][v]['flow']
            capacity = G[u][v]['capacity']
            util = (flow / capacity) if capacity > 0 else 0
            utilization.append(util)
            edge_widths.append(edge_cfg.edge_width_base + flow * edge_cfg.flow_multiplier)

        renderer.draw_edges(
            G, pos, directed,
            width=edge_widths,
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

        legend_elements = [
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
        renderer.add_legend(legend_elements)

        renderer.ax.set_title(
            f'Шаг {snapshot["step"]}: Суммарный поток = {snapshot["total_flow"]:.2f}'
        )
        renderer.ax.axis('off')

    print(f"Создание анимации из {len(snapshots)} кадров...")
    anim = animation.FuncAnimation(
        renderer.fig, update, frames=len(snapshots),
        interval=animation_cfg.animation_interval,
        repeat=True,
        repeat_delay=animation_cfg.animation_repeat_delay
    )

    anim.save(output_file, writer='pillow', fps=1)
    print(f"Анимация сохранена: {output_file}")

    if show:
        webbrowser.open(f'file://{os.path.abspath(output_file)}')

    plt.close()

if __name__ == '__main__':
    args = sys.argv[1:]

    input_file = 'assets/txt/flow_snapshots.txt'
    output_file = 'assets/gif/flow_animation.gif'
    directed = True
    show = False

    i = 0
    while i < len(args):
        if args[i] == '--input' and i + 1 < len(args):
            input_file = args[i + 1]
            i += 2
        elif args[i] == '--output' and i + 1 < len(args):
            output_file = args[i + 1]
            i += 2
        elif args[i] == '--graph_type' and i + 1 < len(args):
            directed = args[i + 1].lower() == 'directed'
            i += 2
        elif args[i] == '--show':
            show = True
            i += 1
        else:
            i += 1

    try:
        snapshots = read_snapshots(input_file)
        print(f"Прочитано {len(snapshots)} snapshots")
        animate_flow_growth(snapshots, output_file, directed, show)
    except FileNotFoundError:
        print(f"Файл {input_file} не найден")
        print("Сначала запустите максимальный поток с логированием")
    except Exception as e:
        print(f"Ошибка: {e}")
