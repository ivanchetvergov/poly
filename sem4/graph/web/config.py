from dataclasses import dataclass
from typing import Dict, List, Optional

@dataclass(frozen=True)
class ActionConfig:
    execute_cmd: Optional[str] = None
    params: Optional[List[str]] = None
    images: List[str] = None

@dataclass(frozen=True)
class LabConfig:
    number: str
    sub_actions: Dict[str, ActionConfig]

LAB_CONFIGS = {
    "Lab 1": LabConfig(
        number="1",
        sub_actions={
            "Generate Graph (Acyclic)": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges", "weight_sign"], images=["01_graph.png"]),
            "Generate Graph (Rice)": ActionConfig(execute_cmd="4", params=["directed", "vertices", "rayleigh_a", "rayleigh_h", "weight_sign"], images=["01_graph.png"]),
            "Adjacency Matrix": ActionConfig(execute_cmd="2", images=["02_adjacency_matrix.png"]),
            "Weight Matrix": ActionConfig(execute_cmd="3", images=["03_weight_matrix.png"]),
            "Shimbell Matrices": ActionConfig(execute_cmd="11", params=["distance"], images=["13_shimbell_min.png", "13_shimbell_max.png"]),
            "Paths": ActionConfig(execute_cmd="12", params=["start_vertex", "end_vertex"], images=["12_paths.png"]),
            "Graph Metrics": ActionConfig(execute_cmd="13", images=["15_graph_metrics.png"]),
        }
    ),
    "Lab 2": LabConfig(
        number="2",
        sub_actions={
            "Generate Graph (Acyclic)": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges", "weight_sign"], images=["01_graph.png"]),
            "Generate Graph (Rice)": ActionConfig(execute_cmd="4", params=["directed", "vertices", "rayleigh_a", "rayleigh_h", "weight_sign"], images=["01_graph.png"]),
            "BFS Traversal": ActionConfig(execute_cmd="21", params=["start_vertex"], images=["21_bfs.gif"]),
            "Dijkstra Path": ActionConfig(execute_cmd="22", params=["start_vertex", "end_vertex"], images=["22_dijkstra.png"]),
            "Compare Algorithms": ActionConfig(execute_cmd="23", params=["start_vertex"], images=["23_compare.png"]),
        }
    ),
    "Lab 3": LabConfig(
        number="3",
        sub_actions={
            "Generate Flow Network": ActionConfig(execute_cmd="31", params=["vertices", "edges", "cost_sign"], images=["31_flow_network.png"]),
            "Generate Flow Network (Rice)": ActionConfig(execute_cmd="36", params=["vertices", "rayleigh_a", "rayleigh_h", "cost_sign"], images=["31_flow_network.png"]),
            "fromGraph": ActionConfig(execute_cmd="37", params=["rayleigh_a", "rayleigh_h"], images=["31_flow_network.png"]),
            "Max Flow": ActionConfig(execute_cmd="32", params=["source", "sink"], images=["32_max_flow.png", "32_flow_animation.gif"]),
            "Min Cost Flow": ActionConfig(execute_cmd="33", params=["source", "sink"], images=["33_min_cost_flow_path.png"]),
            "Capacities": ActionConfig(execute_cmd="34", images=["34_capacity_matrix.png"]),
            "Costs": ActionConfig(execute_cmd="35", images=["35_cost_matrix.png"]),
        }
    ),
    "Lab 4": LabConfig(
        number="4",
        sub_actions={
            "Generate Graph (Acyclic)": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges", "weight_sign"], images=["01_graph.png"]),
            "Generate Graph (Rice)": ActionConfig(execute_cmd="4", params=["directed", "vertices", "rayleigh_a", "rayleigh_h", "weight_sign"], images=["01_graph.png"]),
            "Spanning Trees Count": ActionConfig(execute_cmd="41", images=["41_kirchhoff_matrix_minor.png"]),
            "Minimal Spanning Tree": ActionConfig(execute_cmd="42", images=["41_spanning_tree.png"]),
            "Prufer Encode": ActionConfig(execute_cmd="43", images=["41_spanning_tree.png"]),
            "Prufer Decode": ActionConfig(execute_cmd="44", images=["44_graph.png"]),
            "Min Coloring": ActionConfig(execute_cmd="46", images=["46_coloring.png"]),
        }
    ),
    "Lab 5": LabConfig(
        number="5",
        sub_actions={
            "Generate Graph (Acyclic)": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges", "weight_sign"], images=["01_graph.png"]),
            "Generate Graph (Rice)": ActionConfig(execute_cmd="4", params=["directed", "vertices", "rayleigh_a", "rayleigh_h", "weight_sign"], images=["01_graph.png"]),
            "Eulerian Check + Cycle": ActionConfig(execute_cmd="51", images=["51_graph_original.png", "51_euler_cycle.png"]),
            "Fundamental Cuts (all)": ActionConfig(execute_cmd="52", images=["52_mst.png", "52_cuts.png"]),
            "Fundamental Cuts (subset)": ActionConfig(execute_cmd="53", params=["cut_indices"], images=["52_mst.png", "52_cuts.png"]),
        }
    ),
    "Lab 6": LabConfig(
        number="6",
        sub_actions={
            "HashTable Demo": ActionConfig(execute_cmd="61", images=["61_hashtable_demo.png"]),
            "RBTree Demo": ActionConfig(execute_cmd="62", images=["62_rbtree_demo.png", "65_rbtree_growth.gif"]),
            "HashTable Interactive": ActionConfig(execute_cmd="63", params=["operation", "word"], images=["63_hashtable_interactive.png"]),
            "RBTree Interactive": ActionConfig(execute_cmd="64", params=["operation", "word"], images=["64_rbtree_interactive.png"]),
        }
    ),
}

@dataclass(frozen=True)
class DefaultParams:
    directed: bool = True
    vertices: int = 6
    edges: int = 11
    start_vertex: int = 0
    end_vertex: int = 5
    source: int = 0
    sink: int = 5
    distance: int = 8
    rayleigh_a: int = 1
    rayleigh_h: int = 1
    weight_sign: str = "Positive"
    cost_sign: str = "Positive"

DEFAULT_PARAMS = DefaultParams()
