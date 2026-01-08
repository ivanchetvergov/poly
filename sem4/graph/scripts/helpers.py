def get_path_edges(path, directed):
    path_edges = set()
    if len(path) > 1:
        for i in range(len(path) - 1):
            u, v = path[i], path[i + 1]
            edge = (u, v) if directed else (min(u, v), max(u, v))
            path_edges.add(edge)
    return path_edges

def normalize_edge(u, v, directed):
    return (u, v) if directed else (min(u, v), max(u, v))
