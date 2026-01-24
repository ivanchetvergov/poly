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
    "Lab 1: Shimbell Method and Path Counting": LabConfig(
        number="1",
        sub_actions={
            "Generate Graph": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges"], images=["01_graph.png"]),
            "Adjacency Matrix": ActionConfig(execute_cmd="2", images=["02_adjacency_matrix.png"]),
            "Weight Matrix": ActionConfig(execute_cmd="3", images=["03_weight_matrix.png"]),
            "Shimbell Matrices": ActionConfig(execute_cmd="11", params=["distance"], images=["13_shimbell_min.png", "13_shimbell_max.png"]),
            "Paths": ActionConfig(execute_cmd="12", params=["start_vertex", "end_vertex"], images=["12_paths.png"]),
        }
    ),
    "Lab 2: Placeholder": LabConfig(  # Placeholder
        number="2",
        sub_actions={}
    ),
    "Lab 3: Flows (Max Flow, Min Cost Flow)": LabConfig(
        number="3",
        sub_actions={
            "Generate Flow Network": ActionConfig(execute_cmd="31", params=["vertices", "edges"], images=["31_flow_network.png"]),
            "Max Flow": ActionConfig(execute_cmd="32", params=["source", "sink"], images=["32_max_flow.png", "32_flow_animation.gif"]),
            "Min Cost Flow": ActionConfig(execute_cmd="33", params=["source", "sink"], images=["33_min_cost_flow_path.png"]),
            "Capacities": ActionConfig(execute_cmd="34", images=["34_capacity_matrix.png"]),
            "Costs": ActionConfig(execute_cmd="35", images=["35_cost_matrix.png"]),
        }
    ),
    "Lab 4: Graph Combinatorics": LabConfig(
        number="4",
        sub_actions={
            "Generate Graph": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges"], images=["44_graph.png"]),
            "Spanning Trees": ActionConfig(execute_cmd="41", images=[]),
            "Independent Sets": ActionConfig(execute_cmd="42", images=["42_independent_set.png"]),
            "Independent Edges": ActionConfig(execute_cmd="43", images=["43_independent_edges.png"]),
            "Vertex Cover": ActionConfig(execute_cmd="44", images=["44_vertex_cover.png"]),
            "Edge Cover": ActionConfig(execute_cmd="45", images=["45_edge_cover.png"]),
            "Coloring": ActionConfig(execute_cmd="46", images=["46_coloring.png"]),
        }
    ),
    "Lab 5: Cycles (Eulerian, Hamiltonian, TSP)": LabConfig(
        number="5",
        sub_actions={
            "Generate Graph": ActionConfig(execute_cmd="1", params=["directed", "vertices", "edges"], images=[]),
            "Eulerian Cycle": ActionConfig(execute_cmd="51", images=["51_euler_cycle.png"]),
            "Hamiltonian Cycle": ActionConfig(execute_cmd="52", images=["52_hamilton_cycle.png"]),
            "TSP": ActionConfig(execute_cmd="53", images=["53_tsp_cycle.png"]),
        }
    ),
    "Lab 6: Data Structures (HashTable, RBTree)": LabConfig(
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
    distance: int = 5

DEFAULT_PARAMS = DefaultParams()
