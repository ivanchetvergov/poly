import sys
import networkx as nx
import matplotlib.pyplot as plt

if len(sys.argv) >= 4:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    graph_type = sys.argv[3].lower()
else:
    input_file = 'assets/txt/graph.txt'
    output_file = 'assets/png/path.png'

if graph_type == 'directed':
    G = nx.DiGraph()
else:
    G = nx.Graph()

with open(input_file, 'r') as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) < 2:
            continue
        u, v = int(parts[0]), int(parts[1])
        w = float(parts[2]) if len(parts) > 2 else 1.0
        G.add_edge(u, v, weight=w)

path = []
try:
    with open('assets/txt/path.txt', 'r') as f:
        path = [int(x) for x in f.read().split()]
except FileNotFoundError:
    pass

if G.number_of_nodes() == 0:
    print("[WARN] Граф пустой")
    exit(0)

plt.figure(figsize=(12, 8))
pos = nx.spring_layout(G, seed=42)

edge_colors = []
edge_widths = []
path_edges = set()

if len(path) > 1:
    for i in range(len(path) - 1):
        path_edges.add((min(path[i], path[i+1]), max(path[i], path[i+1])))

for u, v in G.edges():
    edge = (min(u, v), max(u, v))
    if edge in path_edges:
        edge_colors.append('red')
        edge_widths.append(4)
    else:
        edge_colors.append('gray')
        edge_widths.append(1)

nx.draw_networkx_edges(G, pos, edge_color=edge_colors, width=edge_widths, alpha=0.7)

node_colors = []
for node in G.nodes():
    if node in path:
        node_colors.append('orange')
    else:
        node_colors.append('skyblue')

nx.draw_networkx_nodes(G, pos, node_color=node_colors, node_size=700, edgecolors='black')
nx.draw_networkx_labels(G, pos, font_size=12, font_weight='bold')

weights = nx.get_edge_attributes(G, 'weight')
edge_labels = {k: f"{v:.2f}" for k, v in weights.items()}
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='darkblue', font_size=9)

plt.title(f'{title}', fontsize=14, fontweight='bold')
plt.axis('off')
plt.tight_layout()
plt.savefig(output_file, dpi=150)
plt.show()
