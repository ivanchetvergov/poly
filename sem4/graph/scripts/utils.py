"""Общий функционал для всех скриптов визуализации"""

import sys
import networkx as nx
import matplotlib.pyplot as plt
from config import *


def parse_args(default_input, default_output, default_graph_type='undirected'):
    if len(sys.argv) >= 4:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        graph_type = sys.argv[3].lower()
        title = sys.argv[4] if len(sys.argv) >= 5 else None
    else:
        input_file = default_input
        output_file = default_output
        graph_type = default_graph_type
        title = None
    return input_file, output_file, graph_type, title


def parse_args_with_added_edges(default_input, default_output, default_graph_type='undirected'):
    if len(sys.argv) >= 4:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        graph_type = sys.argv[3].lower()
        title = sys.argv[4] if len(sys.argv) >= 5 else None
        added_edges_file = sys.argv[5] if len(sys.argv) >= 6 else None
    else:
        input_file = default_input
        output_file = default_output
        graph_type = default_graph_type
        title = None
        added_edges_file = None
    return input_file, output_file, graph_type, title, added_edges_file


def read_added_edges_file(filename):
    try:
        added_edges = set()
        with open(filename, 'r') as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) >= 2:
                    u, v = int(parts[0]), int(parts[1])
                    added_edges.add((u, v))
        return added_edges
    except (FileNotFoundError, ValueError):
        return set()


def create_graph(graph_type):
    return nx.DiGraph() if graph_type == 'directed' else nx.Graph()


def read_graph_file(filename, G):
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) < 2:
                continue
            u, v = int(parts[0]), int(parts[1])
            w = float(parts[2]) if len(parts) > 2 else 1.0
            G.add_edge(u, v, weight=w)


def read_flow_file(filename, G):
    with open(filename, 'r') as f:
        for line in f:
            if not line.strip():
                continue
            u, v, capacity, flow, cost = line.split()
            u, v = int(u), int(v)
            capacity, flow, cost = float(capacity), float(flow), float(cost)
            G.add_edge(u, v, capacity=capacity, flow=flow, cost=cost)


def read_path_file(filename='assets/txt/path.txt'):
    try:
        with open(filename, 'r') as f:
            return [int(x) for x in f.read().split()]
    except FileNotFoundError:
        return []


def setup_figure():
    plt.figure(figsize=FIGSIZE)


def get_pos(G):
    return nx.spring_layout(G, seed=SEED, k=1.5, iterations=50)


def draw_edges(G, pos, graph_type, **kwargs):
    if graph_type == 'directed':
        return nx.draw_networkx_edges(
            G, pos,
            arrows=True,
            arrowstyle=ARROW_STYLE,
            arrowsize=ARROW_SIZE,
            connectionstyle=CONNECTION_STYLE,
            min_source_margin=MIN_SOURCE_MARGIN,
            min_target_margin=MIN_TARGET_MARGIN,
            **kwargs
        )
    else:
        return nx.draw_networkx_edges(G, pos, **kwargs)


def draw_nodes(G, pos, node_colors=None):
    if node_colors is None:
        node_colors = NODE_COLOR
    nx.draw_networkx_nodes(
        G, pos,
        node_color=node_colors,
        node_size=NODE_SIZE,
        edgecolors=NODE_EDGE_COLOR,
        linewidths=NODE_EDGE_WIDTH
    )


def draw_labels(G, pos):
    nx.draw_networkx_labels(
        G, pos,
        font_size=LABEL_FONT_SIZE,
        font_weight=LABEL_FONT_WEIGHT,
        font_color=LABEL_FONT_COLOR
    )


def draw_edge_labels(G, pos, edge_labels):
    nx.draw_networkx_edge_labels(
        G, pos,
        edge_labels=edge_labels,
        font_color=EDGE_LABEL_FONT_COLOR,
        font_size=EDGE_LABEL_FONT_SIZE,
        font_weight=EDGE_LABEL_FONT_WEIGHT,
        bbox=EDGE_LABEL_BBOX
    )


def finalize_plot(output_file, title=None):
    if title:
        plt.title(title, fontsize=TITLE_FONT_SIZE, fontweight=TITLE_FONT_WEIGHT, pad=20)
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(output_file, bbox_inches='tight')
    plt.show()
    plt.close()


def get_path_edges(path, graph_type):
    path_edges = set()
    if len(path) > 1:
        for i in range(len(path) - 1):
            if graph_type == 'directed':
                path_edges.add((path[i], path[i+1]))
            else:
                path_edges.add((min(path[i], path[i+1]), max(path[i], path[i+1])))
    return path_edges


def normalize_edge(u, v, graph_type):
    if graph_type == 'directed':
        return (u, v)
    else:
        return (min(u, v), max(u, v))
