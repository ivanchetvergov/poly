import networkx as nx

class GraphLoader:

    @staticmethod
    def load_graph(filename: str, directed: bool = False) -> nx.Graph:
        G = nx.DiGraph() if directed else nx.Graph()

        with open(filename, 'r') as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) < 2:
                    continue
                u, v = int(parts[0]), int(parts[1])
                weight = float(parts[2]) if len(parts) > 2 else 1.0
                G.add_edge(u, v, weight=weight)

        return G

    @staticmethod
    def load_flow_network(filename: str, directed: bool = True) -> nx.Graph:
        G = nx.DiGraph() if directed else nx.Graph()

        with open(filename, 'r') as f:
            for line in f:
                if not line.strip():
                    continue
                u, v, capacity, flow, cost = line.split()
                u, v = int(u), int(v)
                capacity, flow, cost = float(capacity), float(flow), float(cost)
                G.add_edge(u, v, capacity=capacity, flow=flow, cost=cost)

        return G

    @staticmethod
    def load_path(filename: str) -> list:
        with open(filename, 'r') as f:
            return [int(x) for x in f.read().strip().split()]

    @staticmethod
    def load_added_edges(filename: str) -> set:
        try:
            with open(filename, 'r') as f:
                added = set()
                for line in f:
                    parts = line.strip().split()
                    if len(parts) >= 2:
                        u, v = int(parts[0]), int(parts[1])
                        added.add((u, v))
                return added
        except FileNotFoundError:
            return set()
