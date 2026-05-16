import sys
import networkx as nx
from scripts.core.renderer import Renderer
from scripts.core.config import plot_cfg, node_cfg, edge_cfg, label_cfg

NIL_PREFIX = '__nil__'


def is_nil_node(node):
    return node.startswith(NIL_PREFIX)


def parse_rbtree_lines(lines):
    nodes = {}
    parents = {}
    children = {}

    for line in lines:
        raw = line.strip()
        if not raw:
            continue

        if '\t' in raw:
            fields = raw.split('\t')
            if len(fields) != 3:
                continue
            node, parent, color = fields
        else:
            parts = raw.split()
            if len(parts) < 3:
                continue
            parent = parts[-2]
            color = parts[-1]
            node = ' '.join(parts[:-2])

        nodes[node] = color
        parents[node] = parent
        if parent != 'null':
            children.setdefault(parent, []).append(node)

    return nodes, parents, children


def attach_nil_children_and_edges(G, node_colors, nodes, children):
    """Attach NIL (black) children to leaf nodes and add all edges.

    Ensures that edges are added in the strictly left-first, right-second order,
    so that hierarchy_pos layout places left children on the left and right on the right.
    """
    for node in nodes:
        child_list = children.get(node, [])
        left_child = None
        right_child = None

        if len(child_list) == 2:
            left_child, right_child = child_list[0], child_list[1]
        elif len(child_list) == 1:
            child = child_list[0]
            # Since strings are compared identically in Python and C++ UTF-8 byte order
            # for basic characters, this safely identifies left vs right child.
            if child < node:
                left_child = child
                right_child = f'{NIL_PREFIX}{node}_R'
                G.add_node(right_child)
                node_colors[right_child] = 'black'
            else:
                left_child = f'{NIL_PREFIX}{node}_L'
                G.add_node(left_child)
                node_colors[left_child] = 'black'
                right_child = child
        else:
            left_child = f'{NIL_PREFIX}{node}_L'
            right_child = f'{NIL_PREFIX}{node}_R'
            G.add_node(left_child)
            node_colors[left_child] = 'black'
            G.add_node(right_child)
            node_colors[right_child] = 'black'

        # MUST add left edge first, right edge second!
        if left_child:
            G.add_edge(node, left_child)
        if right_child:
            G.add_edge(node, right_child)


def build_rbtree_graph_from_text(text):
    nodes, parents, children = parse_rbtree_lines(text.splitlines())

    G = nx.DiGraph()
    node_colors = {}

    for node, color in nodes.items():
        G.add_node(node)
        node_colors[node] = 'red' if color == 'RED' else 'black'

    attach_nil_children_and_edges(G, node_colors, nodes.keys(), children)

    return G, node_colors, parents


def build_rbtree_graph(data_file):
    with open(data_file, 'r', encoding='utf-8') as f:
        return build_rbtree_graph_from_text(f.read())

def hierarchy_pos(G, root, width=1., vert_gap=0.2, vert_loc=1, xcenter=0.5):
    pos = {root: (xcenter, vert_loc)}
    neighbors = list(G.successors(root))
    if len(neighbors) != 0:
        dx = width / len(neighbors)
        nextx = xcenter - width/2 - dx/2
        for neighbor in neighbors:
            nextx += dx
            pos.update(hierarchy_pos(G, neighbor, width=dx, vert_gap=vert_gap, vert_loc=vert_loc-vert_gap, xcenter=nextx))
    return pos

def main():
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "RBTree Visualization"

    G, node_colors, parents = build_rbtree_graph(data_file)

    if not G.nodes():
        renderer = Renderer()
        renderer.setup_plot()
        renderer.finalize(output_file, title + ' (no data)')
        return

    roots = [node for node, parent in parents.items() if parent == 'null']
    if not roots:
        roots = [next(iter(G.nodes()))]

    # Build layout for all roots/components so every node has a position.
    pos = {}
    span = 4.0
    for i, root in enumerate(roots):
        xcenter = i * (span + 1.0)
        pos.update(hierarchy_pos(G, root, width=span, vert_gap=0.5, vert_loc=1, xcenter=xcenter))

    # Safety fallback: place any still-unplaced nodes into a side column.
    missing = [n for n in G.nodes() if n not in pos]
    for i, node in enumerate(missing):
        pos[node] = (len(roots) * (span + 1.0) + 1.0, 1 - i * 0.5)

    renderer = Renderer()
    renderer.setup_plot()

    node_size = node_cfg.rb_node_size

    actual_nodes = [node for node in G.nodes() if not is_nil_node(node)]
    nil_nodes = [node for node in G.nodes() if is_nil_node(node)]

    actual_node_colors = [node_colors.get(node, 'black') for node in actual_nodes]
    if actual_nodes:
        nx.draw_networkx_nodes(G, pos, nodelist=actual_nodes, node_color=actual_node_colors, node_size=node_size)

    if nil_nodes:
        nil_size = max(80, int(node_size * 0.18))
        nx.draw_networkx_nodes(G, pos, nodelist=nil_nodes, node_color='black', node_size=nil_size, linewidths=0.5, edgecolors='gray')

    nx.draw_networkx_edges(G, pos, arrows=True, arrowstyle='->', connectionstyle='arc3,rad=0.0')

    labels = {node: node for node in actual_nodes}
    nx.draw_networkx_labels(G, pos, labels, font_color='white', font_size=label_cfg.label_font_size, font_weight=label_cfg.label_font_weight)

    renderer.finalize(output_file, title)

if __name__ == '__main__':
    main()
