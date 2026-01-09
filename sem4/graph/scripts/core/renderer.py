"""Класс для рендеринга графов"""
from typing import Dict, List, Optional
import matplotlib.pyplot as plt
import networkx as nx

from .config import plot_cfg, node_cfg, edge_cfg, label_cfg


class Renderer:

    def __init__(self):
        self.fig: Optional[plt.Figure] = None
        self.ax: Optional[plt.Axes] = None

    def setup_plot(self):
        self.fig, self.ax = plt.subplots(figsize=plot_cfg.figsize)

    def compute_layout(self, G: nx.Graph) -> Dict:
        return nx.spring_layout(G, seed=plot_cfg.seed, k=1.5, iterations=50)

    def draw_nodes(self, G: nx.Graph, pos: Dict, node_colors=None, **kwargs):
        if node_colors is None:
            node_colors = node_cfg.node_color

        nx.draw_networkx_nodes(
            G, pos,
            node_color=node_colors,
            node_size=node_cfg.node_size,
            edgecolors=node_cfg.node_edge_color,
            linewidths=node_cfg.node_edge_width,
            ax=self.ax,
            **kwargs
        )

    def draw_edges(self, G: nx.Graph, pos: Dict, directed: bool = False, **kwargs):
        if directed:
            nx.draw_networkx_edges(
                G, pos,
                arrows=True,
                arrowstyle=edge_cfg.arrow_style,
                arrowsize=edge_cfg.arrow_size,
                connectionstyle=edge_cfg.connection_style,
                min_source_margin=edge_cfg.min_source_margin,
                min_target_margin=edge_cfg.min_target_margin,
                ax=self.ax,
                **kwargs
            )
        else:
            nx.draw_networkx_edges(G, pos, ax=self.ax, **kwargs)

    def draw_labels(self, G: nx.Graph, pos: Dict):
        nx.draw_networkx_labels(
            G, pos,
            font_size=label_cfg.label_font_size,
            font_weight=label_cfg.label_font_weight,
            font_color=label_cfg.label_font_color,
            ax=self.ax
        )

    def draw_edge_labels(self, G: nx.Graph, pos: Dict, edge_labels: Dict):
        nx.draw_networkx_edge_labels(
            G, pos,
            edge_labels=edge_labels,
            font_color=label_cfg.edge_label_font_color,
            font_size=label_cfg.edge_label_font_size,
            font_weight=label_cfg.edge_label_font_weight,
            bbox=label_cfg.edge_label_bbox,
            ax=self.ax
        )

    def add_legend(self, elements: List):
        self.ax.legend(handles=elements, loc='upper left', fontsize=plot_cfg.fontsize)

    def finalize(self, output_file: str, title: Optional[str] = None, show: bool = True):
        if title:
            self.ax.set_title(
                title,
                fontsize=plot_cfg.title_font_size,
                fontweight=plot_cfg.title_font_weight,
                pad=20
            )
        self.ax.axis('off')
        plt.tight_layout()
        plt.savefig(output_file, bbox_inches='tight')
        if show:
            plt.show()
        plt.close()
