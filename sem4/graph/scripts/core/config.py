from dataclasses import dataclass, field
from typing import Dict, Any


@dataclass(frozen=True)
class PlotConfig:
    figsize: tuple = (12, 8)
    seed: int = 42
    fontsize: int = 10
    title_font_size: int = 15
    title_font_weight: str = 'bold'
    dpi: str = 300


@dataclass(frozen=True)
class NodeConfig:
    node_size: int = 800
    node_color: str = 'skyblue'
    node_edge_color: str = 'black'
    node_edge_width: float = 2.0
    path_node_color: str = 'orange'
    flow_node_color: str = 'lightblue'
    highlight_node_color: str = '#8A2BE2'
    default_node_color: str = '#4682B4'


@dataclass(frozen=True)
class EdgeConfig:
    arrow_style: str = '-|>'
    arrow_size: int = 30
    connection_style: str = 'arc3,rad=0.1'
    min_source_margin: int = 15
    min_target_margin: int = 15
    default_edge_color: str = 'gray'
    default_edge_width: float = 1.5
    default_edge_alpha: float = 0.7
    path_edge_color: str = 'red'
    path_edge_width: float = 4.5
    path_edge_alpha: float = 0.9
    flow_edge_alpha: float = 0.8
    added_edge_color: str = '#C71585'
    added_edge_width: float = 5.0
    highlight_edge_color: str = '#DC143C'
    highlight_edge_width_multiplier: float = 1.5
    background_edge_color: str = '#B0B0B0'
    background_edge_width_multiplier: float = 0.6
    background_edge_alpha: float = 0.7
    edge_width_base: float = 1.5
    flow_multiplier: float = 0.5
    graph_edge_base: float = 0.7
    graph_weight_multiplier: float = 1/3
    path_edge_base: float = 4.5


@dataclass(frozen=True)
class LabelConfig:
    label_font_size: int = 13
    label_font_weight: str = 'bold'
    label_font_color: str = 'black'
    edge_label_font_size: int = 10
    edge_label_font_color: str = 'darkred'
    edge_label_font_weight: str = 'normal'
    edge_label_bbox: Dict[str, Any] = field(default_factory=lambda: {
        'boxstyle': 'round,pad=0.3',
        'facecolor': 'white',
        'edgecolor': 'none',
        'alpha': 0.8
    })


@dataclass(frozen=True)
class ColormapConfig:
    graph_colormap: str = 'magma'
    flow_colormap: str = 'RdYlGn_r'


@dataclass(frozen=True)
class LegendConfig:
    legend_node_marker: str = 'o'
    legend_edge_style: str = '-'
    legend_node_size: int = 14


@dataclass(frozen=True)
class MatrixConfig:
    matrix_figsize: tuple = (10, 10)
    matrix_colormap: str = 'viridis'
    matrix_title_size: int = 15
    matrix_annot_size: int = 10


@dataclass(frozen=True)
class AnimationConfig:
    animation_interval: int = 500
    animation_repeat_delay: int = 1000
    dpi: int = 300
    fps: int = 2


plot_cfg = PlotConfig()
node_cfg = NodeConfig()
edge_cfg = EdgeConfig()
label_cfg = LabelConfig()
colormap_cfg = ColormapConfig()
legend_cfg = LegendConfig()
matrix_cfg = MatrixConfig()
animation_cfg = AnimationConfig()

