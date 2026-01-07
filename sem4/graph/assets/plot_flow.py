import sys
import networkx as nx
import matplotlib.pyplot as plt

if len(sys.argv) >= 3:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
else:
    input_file = 'assets/flow.txt'
    output_file = 'assets/flow.png'

G = nx.Graph()
flow_data = {}

# Формат файла: from to capacity flow cost
with open(input_file, 'r') as f:
    for line in f:
        parts = line.split()
        u, v = int(parts[0]), int(parts[1])
        capacity = float(parts[2])
        flow = float(parts[3])
        cost = float(parts[4])
        G.add_edge(u, v, capacity=capacity, flow=flow, cost=cost)
        flow_data[(u, v)] = {'capacity': capacity, 'flow': flow, 'cost': cost}

plt.figure(figsize=(12, 8))
pos = nx.spring_layout(G, seed=42)

# Рёбра: толщина по потоку, цвет по загрузке (flow/capacity)
edges = G.edges()
flows = [G[u][v]['flow'] for u, v in edges]
capacities = [G[u][v]['capacity'] for u, v in edges]
utilization = [G[u][v]['flow'] / G[u][v]['capacity'] if G[u][v]['capacity'] > 0 else 0 for u, v in edges]

# Рисуем рёбра с цветом по утилизации (flow/capacity)
nx.draw_networkx_edges(
    G, pos,
    width=[1 + f/2 for f in flows],
    edge_color=utilization,
    edge_cmap=plt.cm.RdYlGn_r,  # Красный = полная загрузка, зелёный = низкая
    edge_vmin=0,
    edge_vmax=1
)

# Вершины
nx.draw_networkx_nodes(G, pos, node_color='lightblue', node_size=700, edgecolors='black')
nx.draw_networkx_labels(G, pos, font_size=12, font_weight='bold')

# Подписи рёбер: flow/capacity
edge_labels = {(u, v): f"{G[u][v]['flow']:.1f}/{G[u][v]['capacity']:.1f}" for u, v in edges}
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='darkred', font_size=9)

plt.title('Визуализация потока (толщина = поток, цвет = загрузка)', fontsize=14)
plt.axis('off')
plt.tight_layout()
plt.savefig(output_file, dpi=150)
plt.show()
