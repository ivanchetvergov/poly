import base64
import os
import shutil
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import streamlit as st
from PIL import Image
from scipy.special import i0
from scipy.stats import gaussian_kde

from config import LAB_CONFIGS, DEFAULT_PARAMS

PROJECT_ROOT = Path(__file__).parent.parent
BUILD_DIR = PROJECT_ROOT / "build"
ASSETS_DIR = PROJECT_ROOT / "assets"
EXECUTABLE = BUILD_DIR / "graph_main"


class GraphLabApp:
    LABS = ["Lab 1", "Lab 2", "Lab 3", "Lab 4", "Lab 5", "Lab 6"]
    DISABLED_LABS = {"Lab 3", "Lab 4", "Lab 5", "Lab 6"}

    def __init__(self):
        self._s = st.session_state

    # ------------------------------------------------------------------
    # Process management
    # ------------------------------------------------------------------

    def send_command(self, lab: str, command: str) -> None:
        print(f"DEBUG: Sending command: {command}")
        if lab not in self._s.processes:
            return
        proc = self._s.processes[lab]['proc']
        if proc.poll() is None:
            proc.stdin.write(command + '\n')
            proc.stdin.flush()

    def kill_process(self, lab: str) -> None:
        if lab in self._s.processes:
            proc = self._s.processes[lab]['proc']
            if proc.poll() is None:
                proc.terminate()
                proc.wait()
            del self._s.processes[lab]

    def is_process_running(self, lab: str) -> bool:
        if lab in self._s.processes:
            return self._s.processes[lab]['proc'].poll() is None
        return False

    def start_process(self, lab: str) -> None:
        if lab in self._s.processes:
            return
        proc = subprocess.Popen(
            [str(EXECUTABLE)],
            stdin=subprocess.PIPE,
            text=True,
            cwd=PROJECT_ROOT,
        )
        self._s.processes[lab] = {'proc': proc, 'output': ''}

    # ------------------------------------------------------------------
    # State helpers
    # ------------------------------------------------------------------

    def _get(self, key: str, default=None):
        return self._s.get(key, default)

    def _set(self, key: str, value) -> None:
        self._s[key] = value

    def _param_key(self, lab: str, action: str, param: str) -> str:
        return f"{lab}_{action}_{param}"

    def _get_param(self, lab: str, action: str, param: str, default):
        return self._s.get(self._param_key(lab, action, param), default)

    def _current_vertices(self, lab: str) -> int:
        return self._get(f"{lab}_current_vertices", DEFAULT_PARAMS.vertices)

    def _current_edges(self, lab: str) -> int:
        return self._get(f"{lab}_current_edges", DEFAULT_PARAMS.edges)

    # ------------------------------------------------------------------
    # Distribution visualization
    # ------------------------------------------------------------------

    @staticmethod
    def _read_weights(graph_txt: Path) -> list[float]:
        weights = []
        if not graph_txt.exists():
            return weights
        with open(graph_txt) as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) == 3:
                    try:
                        weights.append(float(parts[2]))
                    except ValueError:
                        pass
        return weights

    def plot_weight_distribution(self) -> None:
        dist_type = self._get('dist_type')
        if not dist_type:
            return
        weights = self._read_weights(ASSETS_DIR / 'txt' / '01_graph.txt')
        if not weights:
            return

        a = self._get('dist_a', DEFAULT_PARAMS.rayleigh_a)
        h = self._get('dist_h', DEFAULT_PARAMS.rayleigh_h)

        fig, ax = plt.subplots(figsize=(6, 3))
        ax.hist(weights, bins=20, density=True, alpha=0.35, color='steelblue', label='Гистограмма')

        x_data = np.linspace(max(0, min(weights) - 1), max(weights) * 1.1, 400)
        kde = gaussian_kde(weights)
        ax.plot(x_data, kde(x_data), 'b-', lw=2, label='KDE (данные)')

        if dist_type == 'uniform':
            x_pdf = np.linspace(0.0, 11.0, 400)
            pdf = np.where((x_pdf >= 1.0) & (x_pdf <= 10.0), 1.0 / 9.0, 0.0)
            ax.plot(x_pdf, pdf, 'r--', lw=2, label='U[1, 10] PDF')
            ax.set_title('Распределение весов: равномерное U[1, 10]')
        elif dist_type == 'rice':
            sigma, nu = float(a), float(h)
            if sigma > 0:
                x_pdf = np.linspace(0.0, (nu + 5 * sigma) * 1.2, 400)
                pdf = (x_pdf / sigma ** 2) * np.exp(-(x_pdf ** 2 + nu ** 2) / (2 * sigma ** 2)) * i0(x_pdf * nu / sigma ** 2)
                ax.plot(x_pdf, pdf, 'r--', lw=2, label=f'Rice(a={sigma}, h={nu}) PDF')
            ax.set_title(f'Распределение весов: Райс (a={sigma}, h={nu})')

        ax.set_xlabel('Вес')
        ax.set_ylabel('Плотность')
        ax.legend()
        fig.tight_layout()
        st.pyplot(fig)
        plt.close(fig)

    # ------------------------------------------------------------------
    # Image display
    # ------------------------------------------------------------------

    def display_image(self, img_path: Path) -> None:
        if not (img_path.exists() and os.path.getsize(str(img_path)) > 0):
            return
        try:
            with Image.open(str(img_path)) as im:
                im.verify()
            if img_path.suffix == '.gif':
                with open(str(img_path), "rb") as f:
                    data = base64.b64encode(f.read()).decode()
                st.markdown(
                    f'<img src="data:image/gif;base64,{data}" '
                    f'style="width:100%; background-color: white;" loop="infinite" autoplay>',
                    unsafe_allow_html=True,
                )
            else:
                st.image(str(img_path))
        except Exception:
            pass

    def _display_visualizations(self) -> None:
        images = self._s.current_visualization
        dist_type = self._get('dist_type')
        show_dist = dist_type and any('01_graph' in img for img in (images or []))

        if not images and not show_dist:
            return

        if show_dist:
            col_img, col_dist = st.columns(2)
            with col_img:
                paths = [ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img for img in (images or [])]
                for p in paths:
                    self.display_image(p)
            with col_dist:
                self.plot_weight_distribution()
        elif images:
            paths = [ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img for img in images]
            if len(paths) == 2:
                col1, col2 = st.columns(2)
                with col1:
                    self.display_image(paths[0])
                with col2:
                    self.display_image(paths[1])
            else:
                for p in paths:
                    self.display_image(p)

    # ------------------------------------------------------------------
    # Main menu
    # ------------------------------------------------------------------

    def render_main_menu(self) -> None:
        st.markdown("<h2 style='text-align: center;'>Theory Graph Labs</h2>", unsafe_allow_html=True)
        st.markdown(
            '<style>.stButton button { margin: 0 !important; padding: 2rem 4rem !important; '
            'font-size: 5rem !important; height: 7.2rem !important; width: 500px !important; }</style>',
            unsafe_allow_html=True,
        )
        st.markdown('<div style="display: flex; justify-content: center;"><div style="max-width: 800px;">', unsafe_allow_html=True)

        cols = st.columns(3, gap="small")
        for i, lab_name in enumerate(self.LABS):
            with cols[i % 3]:
                if lab_name in self.DISABLED_LABS:
                    st.button(lab_name, disabled=True, key=f"select_{lab_name}")
                elif self.is_process_running(lab_name):
                    st.error(f"{lab_name} (Running)")
                    if st.button(f"Stop {lab_name}", key=f"stop_{lab_name}"):
                        self.kill_process(lab_name)
                        st.rerun()
                else:
                    if st.button(lab_name, key=f"select_{lab_name}"):
                        self.start_process(lab_name)
                        self._s.current_lab = lab_name
                        self._set(f"{lab_name}_graph_generated", False)
                        self._s.current_visualization = []
                        self._set('dist_type', None)
                        self._clear_assets()
                        st.rerun()

        st.markdown('</div></div>', unsafe_allow_html=True)

    def _clear_assets(self) -> None:
        for dir_name in ["png", "gif"]:
            dir_path = ASSETS_DIR / dir_name
            if dir_path.exists():
                shutil.rmtree(dir_path)
            dir_path.mkdir()

    # ------------------------------------------------------------------
    # Param widget rendering
    # ------------------------------------------------------------------

    def _render_param_widget(self, lab: str, action: str, param: str) -> None:
        key = self._param_key(lab, action, param)
        v = self._current_vertices(lab)
        e = self._current_edges(lab)
        if param == "directed":
            st.checkbox("Directed", value=DEFAULT_PARAMS.directed, key=key)
        elif param == "vertices":
            st.number_input("Vertices", 3, 50, DEFAULT_PARAMS.vertices, key=key)
        elif param == "edges":
            st.number_input("Edges", 5, 100, DEFAULT_PARAMS.edges, key=key)
        elif param == "start_vertex":
            if self._s.get(key, 0) > v - 1:
                self._s[key] = v - 1
            st.number_input("Start Vertex", 0, v - 1, min(DEFAULT_PARAMS.start_vertex, v - 1), key=key)
        elif param == "end_vertex":
            if self._s.get(key, DEFAULT_PARAMS.end_vertex) > v - 1:
                self._s[key] = v - 1
            st.number_input("End Vertex", 0, v - 1, min(DEFAULT_PARAMS.end_vertex, v - 1), key=key)
        elif param == "source":
            st.number_input("Source", 0, v - 1, min(DEFAULT_PARAMS.source, v - 1), key=key)
        elif param == "sink":
            st.number_input("Sink", 0, v - 1, min(DEFAULT_PARAMS.sink, v - 1), key=key)
        elif param == "distance":
            st.number_input("Distance", 0, e, min(DEFAULT_PARAMS.distance, e), key=key)
        elif param == "rayleigh_a":
            st.number_input("a (масштаб, a > 0)", 1, 100, DEFAULT_PARAMS.rayleigh_a, key=key)
        elif param == "rayleigh_h":
            st.number_input("h (форма, h > 0)", 1, 100, DEFAULT_PARAMS.rayleigh_h, key=key)
        elif param == "weight_sign":
            st.selectbox("Знак весов", ["Positive", "Negative", "Mixed"],
                         index=["Positive", "Negative", "Mixed"].index(DEFAULT_PARAMS.weight_sign), key=key)
        elif param == "cost_sign":
            st.selectbox("Знак стоимостей", ["Positive", "Negative", "Mixed"],
                         index=["Positive", "Negative", "Mixed"].index(DEFAULT_PARAMS.cost_sign), key=key)
        elif param == "operation":
            st.selectbox("Operation", ["insert", "delete", "search"], key=key)
        elif param == "word":
            st.text_input("Word", key=key)

    # ------------------------------------------------------------------
    # Build params string for stdin
    # ------------------------------------------------------------------

    def _build_params_str(self, lab: str, action: str, params: list[str]) -> str:
        parts: list[str] = []
        handlers = {
            "directed":     lambda: "1" if self._get_param(lab, action, "directed", DEFAULT_PARAMS.directed) else "0",
            "vertices":     lambda: str(self._get_param(lab, action, "vertices", DEFAULT_PARAMS.vertices)),
            "edges":        lambda: str(self._get_param(lab, action, "edges", DEFAULT_PARAMS.edges)),
            "start_vertex": lambda: str(self._get_param(lab, action, "start_vertex", DEFAULT_PARAMS.start_vertex)),
            "end_vertex":   lambda: str(self._get_param(lab, action, "end_vertex", DEFAULT_PARAMS.end_vertex)),
            "source":       lambda: str(self._get_param(lab, action, "source", DEFAULT_PARAMS.source)),
            "sink":         lambda: str(self._get_param(lab, action, "sink", DEFAULT_PARAMS.sink)),
            "distance":     lambda: str(self._get_param(lab, action, "distance", DEFAULT_PARAMS.distance)),
            "rayleigh_a":   lambda: str(self._get_param(lab, action, "rayleigh_a", DEFAULT_PARAMS.rayleigh_a)),
            "rayleigh_h":   lambda: str(self._get_param(lab, action, "rayleigh_h", DEFAULT_PARAMS.rayleigh_h)),
            "weight_sign":  lambda: str(["Positive", "Negative", "Mixed"].index(
                                self._get_param(lab, action, "weight_sign", DEFAULT_PARAMS.weight_sign))),
            "cost_sign":    lambda: str(["Positive", "Negative", "Mixed"].index(
                                self._get_param(lab, action, "cost_sign", DEFAULT_PARAMS.cost_sign))),
            "operation":    lambda: str(self._get_param(lab, action, "operation", "insert")),
            "word":         lambda: str(self._get_param(lab, action, "word", "")),
        }
        for p in params:
            if p not in handlers:
                continue
            value = handlers[p]()
            parts.append(value)
            if p == "vertices":
                self._set(f"{lab}_current_vertices", int(value))
            elif p == "edges":
                self._set(f"{lab}_current_edges", int(value))
            elif p == "sink" and action == "Min Cost Flow":
                parts.append("1")  # auto-confirm default 2/3 of max flow
        return "\n".join(parts)

    # ------------------------------------------------------------------
    # Lab 6 rendering
    # ------------------------------------------------------------------

    def _render_lab6_actions(self, lab: str, lab_config) -> None:
        is_any_open = any(
            self._get(f"{lab}_{act}_open", False)
            for act in lab_config.sub_actions
            if "Interactive" in act
        )
        for action_name, action_config in lab_config.sub_actions.items():
            open_key = f"{lab}_{action_name}_open"
            if "Interactive" in action_name:
                self._render_interactive_action(lab, action_name, action_config, open_key, is_any_open)
            else:
                self._render_lab6_simple_action(lab, action_name, action_config, is_any_open)

    def _render_interactive_action(self, lab: str, action_name: str, action_config, open_key: str, is_any_open: bool) -> None:
        disabled_open = is_any_open and not self._get(open_key, False)
        if st.button(f"Open {action_name}", key=f"open_{action_name}", disabled=disabled_open):
            self._set(open_key, True)
        if not self._get(open_key, False):
            return
        entered_key = f"{open_key}_entered"
        if not self._get(entered_key, False):
            self.send_command(lab, str(action_config.execute_cmd))
            self._set(entered_key, True)
        with st.container():
            with st.form(key=f"{lab}_{action_name}_form"):
                operation = st.selectbox("Operation", ["insert", "remove", "search"])
                word = st.text_input("Word")
                if st.form_submit_button("Submit Command"):
                    self.send_command(lab, f"{operation} {word}")
            if st.button("Exit from interactive loop", key=f"{lab}_{action_name}_exit"):
                self.send_command(lab, "exit")
                self._set(open_key, False)
                self._set(entered_key, False)
                st.rerun()
            col1, col2 = st.columns(2, gap="small")
            with col1:
                if st.button("Visualize", key=f"{lab}_{action_name}_draw"):
                    self.send_command(lab, "draw")
                    self._s.current_visualization = action_config.images
            with col2:
                if "RBTree" in action_name and st.button("GIF", key=f"{lab}_{action_name}_gif"):
                    self.send_command(lab, "gif")
                    self._s.current_visualization = ["65_rbtree_growth.gif"]

    def _render_lab6_simple_action(self, lab: str, action_name: str, action_config, is_any_open: bool) -> None:
        if is_any_open:
            st.write(f"{action_name} (disabled while interactive mode is open)")
            return
        with st.expander(action_name):
            col1, col2 = st.columns(2, gap="small")
            with col1:
                if st.button("Execute", key=f"{lab}_{action_name}_execute"):
                    with st.spinner("Executing..."):
                        self.send_command(lab, action_config.execute_cmd)
            with col2:
                if st.button("Visualize", key=f"{lab}_{action_name}_visualize"):
                    self._s.current_visualization = action_config.images

    # ------------------------------------------------------------------
    # Default lab rendering
    # ------------------------------------------------------------------

    def _render_default_actions(self, lab: str, lab_config) -> None:
        for action_name, action_config in lab_config.sub_actions.items():
            with st.expander(action_name):
                for param in action_config.params or []:
                    self._render_param_widget(lab, action_name, param)

                is_gen = "Generate" in action_name
                needs_maxflow = action_name == "Min Cost Flow"
                disabled = (
                    not is_gen and not self._get(f'{lab}_graph_generated', False)
                ) or (
                    needs_maxflow and not self._get(f'{lab}_maxflow_done', False)
                )

                col1, col2 = st.columns(2, gap="small")
                with col1:
                    if st.button("Execute", disabled=disabled, key=f"{lab}_{action_name}_execute"):
                        with st.spinner("Executing..."):
                            params_str = self._build_params_str(lab, action_name, action_config.params or [])
                            cmd = action_config.execute_cmd
                            full_cmd = f"{cmd}\n{params_str}" if params_str else cmd
                            self.send_command(lab, full_cmd)
                            if is_gen:
                                self._set(f'{lab}_graph_generated', True)
                                self._set(f'{lab}_maxflow_done', False)
                                self._s.current_visualization = action_config.images
                                if 'Rice' in action_name:
                                    self._set('dist_type', 'rice')
                                    self._set('dist_a', self._get_param(lab, action_name, 'rayleigh_a', DEFAULT_PARAMS.rayleigh_a))
                                    self._set('dist_h', self._get_param(lab, action_name, 'rayleigh_h', DEFAULT_PARAMS.rayleigh_h))
                                elif 'Acyclic' in action_name:
                                    self._set('dist_type', 'uniform')
                            if action_name == "Max Flow":
                                self._set(f'{lab}_maxflow_done', True)
                with col2:
                    if st.button("Visualize", disabled=disabled, key=f"{lab}_{action_name}_visualize"):
                        self._s.current_visualization = action_config.images

    # ------------------------------------------------------------------
    # Lab UI entry point
    # ------------------------------------------------------------------

    def render_lab_ui(self, lab_name: str) -> None:
        lab_config = LAB_CONFIGS[lab_name]
        with st.sidebar:
            st.header(lab_name)
            if st.button("Back to Main menu", key="back_to_menu"):
                self.send_command(lab_name, "0")
                self.kill_process(lab_name)
                self._s.current_lab = None
                st.rerun()
            if not self.is_process_running(lab_name):
                st.error("Process not running. Go back and restart.")
                return
            if lab_name == "Lab 6":
                self._render_lab6_actions(lab_name, lab_config)
            else:
                self._render_default_actions(lab_name, lab_config)
        self._display_visualizations()

    # ------------------------------------------------------------------
    # Main entry point
    # ------------------------------------------------------------------

    def run(self) -> None:
        lab = self._s.get('current_lab')
        if lab:
            self.render_lab_ui(lab)
        else:
            self.render_main_menu()

