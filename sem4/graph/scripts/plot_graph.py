import sys
import networkx as nx
import matplotlib.pyplot as plt

if len(sys.argv) >= 3:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
else:
    input_file = 'assets/txt/graph.txt'
    output_file = 'assets/png/graph.png'

G = nx.Graph()
with open(input_file, 'r') as f:
    for line in f:
        u, v, w = line.split()
        G.add_edge(int(u), int(v), weight=float(w))

plt.figure(figsize=(12, 8))
pos = nx.spring_layout(G, seed=42)

weights = [G[u][v]['weight'] for u, v in G.edges()]
nx.draw_networkx_edges(
    G, pos,
    width=[0.7 + w/3 for w in weights],
    edge_color=weights,
    edge_cmap=plt.cm.magma)

nx.draw_networkx_nodes(G, pos, node_color='skyblue', node_size=600, edgecolors='black')
nx.draw_networkx_labels(G, pos, font_size=12, font_weight='bold')

edge_labels = nx.get_edge_attributes(G, 'weight')
edge_labels = {k: f"{v:.2f}" for k, v in edge_labels.items()}
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='darkred', font_size=10)

plt.axis('off')
plt.tight_layout()
plt.savefig(output_file)
plt.show()
