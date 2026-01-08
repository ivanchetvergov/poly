def get_path_edges(path, directed):
    path_edges = set()
    if len(path) > 1:
        for i in range(len(path) - 1):
            u, v = path[i], path[i + 1]
            edge = (u, v) if directed else (min(u, v), max(u, v))
            path_edges.add(edge)
    return path_edges

def read_paths(filename):
    with open(filename, 'r') as f:
        num_paths = int(f.readline().strip())
        paths = []
        for _ in range(num_paths):
            path = [int(x) for x in f.readline().strip().split()]
            paths.append(path)
        return paths

def normalize_edge(u, v, directed):
    return (u, v) if directed else (min(u, v), max(u, v))
