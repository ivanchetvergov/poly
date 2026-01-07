import sys
import networkx as nx
import matplotlib.pyplot as plt

if len(sys.argv) >= 3:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
else:
    input_file = 'assets/txt/flow.txt'
    output_file = 'assets/png/flow.png'

G = nx.Graph()
with open(input_file, 'r') as f:
    for line in f:
        if not line.strip():
            continue
        u, v, capacity, flow, cost = line.split()
        u, v = int(u), int(v)
        capacity, flow, cost = float(capacity), float(flow), float(cost)
        G.add_edge(u, v, capacity=capacity, flow=flow, cost=cost)

plt.figure(figsize=(12, 8))
pos = nx.spring_layout(G, seed=42)

edges = list(G.edges())
flows = [G[u][v]['flow'] for u, v in edges]
capacities = [G[u][v]['capacity'] for u, v in edges]
utilization = [
    (G[u][v]['flow'] / G[u][v]['capacity']) if G[u][v]['capacity'] > 0 else 0
    for u, v in edges
]

nx.draw_networkx_edges(
    G, pos,
    width=[1 + f / 2 for f in flows],
    edge_color=utilization,
    edge_cmap=plt.cm.RdYlGn_r,
    edge_vmin=0,
    edge_vmax=1
)

nx.draw_networkx_nodes(G, pos, node_color='lightblue', node_size=700, edgecolors='black')
nx.draw_networkx_labels(G, pos, font_size=12, font_weight='bold')

edge_labels = {
    (u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}"
    for u, v in edges
}
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='darkred', font_size=9)

plt.title('Визуализация потока (толщина = поток, цвет = загрузка)', fontsize=14)
plt.axis('off')
plt.tight_layout()
plt.savefig(output_file, dpi=150)
plt.show()
