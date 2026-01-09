def get_path_edges(path, directed):
    path_edges = set()
    if len(path) > 1:
        for i in range(len(path) - 1):
            u, v = path[i], path[i + 1]
            edge = (u, v) if directed else (min(u, v), max(u, v))
            path_edges.add(edge)
    return path_edges

def read_paths(filename):
    try:
        with open(filename, 'r') as f:
            content = f.read().strip()
            if not content:
                return []

            lines = content.split('\n')
            if not lines:
                return []

            try:
                num_paths = int(lines[0])
            except ValueError:
                return []

            paths = []
            for i in range(1, min(num_paths + 1, len(lines))):
                line = lines[i].strip()
                if line:
                    try:
                        path = [int(x) for x in line.split()]
                        if path:
                            paths.append(path)
                    except ValueError:
                        continue
            return paths
    except FileNotFoundError:
        return []

def normalize_edge(u, v, directed):
    return (u, v) if directed else (min(u, v), max(u, v))
