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
    DISABLED_LABS = {}

    def __init__(self):
        self._s = st.session_state
        self.start_process(None)

    # ------------------------------------------------------------------
    # Process management
    # ------------------------------------------------------------------

    def send_command(self, lab: str, command: str) -> None:
        if 'cpp_process' not in self._s:
            return
        proc = self._s.cpp_process['proc']
        if proc.poll() is None:
            proc.stdin.write(command + '\n')
            proc.stdin.flush()

    def kill_process(self, lab: str) -> None:
        if 'cpp_process' in self._s:
            proc = self._s.cpp_process['proc']
            if proc.poll() is None:
                proc.terminate()
                proc.wait()
            del self._s['cpp_process']

    def is_process_running(self, lab: str) -> bool:
        if 'cpp_process' in self._s:
            return self._s.cpp_process['proc'].poll() is None
        return False

    def start_process(self, lab: str) -> None:
        if self.is_process_running(lab):
            return
        env = os.environ.copy()
        env["GRAPH_WEB_MODE"] = "1"
        proc = subprocess.Popen(
            [str(EXECUTABLE)],
            stdin=subprocess.PIPE,
            text=True,
            cwd=PROJECT_ROOT,
            env=env,
        )
        self._s.cpp_process = {'proc': proc}

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
    def _read_values(data_txt: Path, value_index: int, min_parts: int) -> list[float]:
        values = []
        if not data_txt.exists():
            return values
        with open(data_txt) as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) >= min_parts:
                    try:
                        values.append(float(parts[value_index]))
                    except ValueError:
                        pass
        return values

    def plot_weight_distribution(self) -> None:
        dist_type = self._get('dist_type')
        if not dist_type:
            return
        dist_source = self._get('dist_source', 'graph')

        if dist_source == 'flow':
            values = self._read_values(ASSETS_DIR / 'txt' / '31_flow.txt', value_index=4, min_parts=5)
            x_label = 'Стоимость ребра'
            chart_title_prefix = 'Распределение стоимостей'
        else:
            values = self._read_values(ASSETS_DIR / 'txt' / '01_graph.txt', value_index=2, min_parts=3)
            x_label = 'Вес'
            chart_title_prefix = 'Распределение весов'

        weights = values
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
            ax.set_title(f'{chart_title_prefix}: равномерное U[1, 10]')
        elif dist_type == 'rice':
            sigma, nu = float(a), float(h)
            if sigma > 0:
                x_pdf = np.linspace(0.0, (nu + 5 * sigma) * 1.2, 400)
                pdf = (x_pdf / sigma ** 2) * np.exp(-(x_pdf ** 2 + nu ** 2) / (2 * sigma ** 2)) * i0(x_pdf * nu / sigma ** 2)
                ax.plot(x_pdf, pdf, 'r--', lw=2, label=f'Rice(a={sigma}, h={nu}) PDF')
            ax.set_title(f'{chart_title_prefix}: Райс (a={sigma}, h={nu})')

        ax.set_xlabel(x_label)
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
                    f'style="width:100%; display:block; background-color: white;" loop="infinite" autoplay>',
                    unsafe_allow_html=True,
                )
            else:
                st.image(str(img_path), width='stretch')
        except Exception:
            pass

    def _read_euler_path_text(self) -> str:
        path_file = ASSETS_DIR / 'txt' / '51_path.txt'
        if not path_file.exists():
            return ""
        try:
            with open(path_file, 'r', encoding='utf-8') as f:
                lines = [line.strip() for line in f if line.strip()]
            if len(lines) < 2:
                return ""
            vertices = lines[1].split()
            if not vertices:
                return ""
            return "[PATH] " + " -> ".join(vertices)
        except Exception:
            return ""

    def _display_euler_path_badge(self, images: list[str] | None) -> None:
        if not images or '51_euler_cycle.png' not in images:
            return
        path_text = self._read_euler_path_text()
        if not path_text:
            return
        st.markdown(
            (
                '<div style="margin-top: 0.75rem; padding: 0.45rem 0.7rem; '
                'border: 1px solid #b8c0cc; border-radius: 8px; '
                'font-family: monospace; font-size: 0.82rem; color: #111827; '
                'background: #f3f4f6; '
                'display: inline-block;">'
                f'{path_text}'
                '</div>'
            ),
            unsafe_allow_html=True,
        )

    def _read_text_report(self, report_path: str | None) -> str:
        if not report_path:
            return ""
        path = ASSETS_DIR / 'txt' / report_path
        if not path.exists():
            return ""
        try:
            with open(path, 'r', encoding='utf-8') as f:
                return f.read().strip()
        except Exception:
            return ""

    def _display_text_report(self, images: list[str] | None) -> None:
        report_path = self._s.get('current_report_file')
        if not report_path:
            return
        report_text = self._read_text_report(report_path)
        if not report_text:
            return
        if not images:
            st.code(report_text, language="text")
            return
        st.code(report_text, language="text")

    def _display_visualizations(self) -> None:
        images = self._s.current_visualization
        dist_type = self._get('dist_type')
        show_dist = dist_type and any(('01_graph' in img) or ('31_flow_network' in img) for img in (images or []))

        if not images and not show_dist:
            return

        if show_dist:
            col_img, col_dist = st.columns([3, 2])
            with col_img:
                paths = [ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img for img in (images or [])]
                for p in paths:
                    self.display_image(p)
            with col_dist:
                self.plot_weight_distribution()
        elif images:
            paths = [ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img for img in images]
            if len(paths) == 2:
                image_cols = st.columns(2, gap="large")
                with image_cols[0]:
                    self.display_image(paths[0])
                with image_cols[1]:
                    self.display_image(paths[1])
            else:
                for p in paths:
                    self.display_image(p)

        self._display_euler_path_badge(images)
        self._display_text_report(images)

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
                else:
                    if st.button(lab_name, key=f"select_{lab_name}"):
                        self.start_process(lab_name)
                        self._s.current_lab = lab_name
                        self._s.current_visualization = []
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
            st.number_input("Vertices", 2, 1000, DEFAULT_PARAMS.vertices, key=key)
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
        elif param == "cut_indices":
            st.text_input("Номера разрезов (через пробел или запятую)", key=key)

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
            "cut_indices":  lambda: str(self._get_param(lab, action, "cut_indices", "")).strip() or "empty",
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

        if "HashTable" in action_name:
            with st.expander("Configuration", expanded=False):
                with st.form(key=f"{lab}_{action_name}_cap_form"):
                    cap = st.number_input("Capacity (resets table)", min_value=1, max_value=100000, value=20, key=f"{lab}_{action_name}_cap")
                    if st.form_submit_button("Set Capacity"):
                        self.send_command(lab, f"capacity {cap}")

        # Section for file loading
        with st.expander("Load from File", expanded=False):
            uploaded_file = st.file_uploader("Choose a text file", type=['txt'], key=f"{lab}_{action_name}_file")

            if st.button("Load File", key=f"{lab}_{action_name}_load_file"):
                if uploaded_file is not None:
                    try:
                        # Save uploaded file temporarily
                        safe_action_name = action_name.replace(" ", "_")
                        temp_path = str(PROJECT_ROOT / f"temp_{safe_action_name}.txt")
                        with open(temp_path, 'w', encoding='utf-8') as f:
                            f.write(uploaded_file.getvalue().decode('utf-8'))

                        # Send load command to C++ process (always uses unigrams)
                        self.send_command(lab, f"load {temp_path}")
                        st.success("File loaded successfully! Tree properties are maintained via lexicographic ordering of words (n-grams).")
                    except Exception as e:
                        st.error(f"Error loading file: {str(e)}")
                else:
                    st.warning("Please select a file first")

        with st.container():
            with st.form(key=f"{lab}_{action_name}_form"):
                operation = st.selectbox("Operation", ["insert", "remove", "search", "clear"], key=f"{lab}_{action_name}_op")
                word = st.text_input("Word", key=f"{lab}_{action_name}_word")

                if st.form_submit_button("Submit", use_container_width=True):
                    if operation == "clear":
                        self.send_command(lab, "clear")
                    elif word:
                        self.send_command(lab, f"{operation} {word}")
                    else:
                        st.warning("Please enter a word")

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
                    # GIF file might not exist if no operations were performed
                    if "65_rbtree_growth.gif" in action_config.images:
                        self._s.current_visualization = ["65_rbtree_growth.gif"]
                    else:
                        st.warning("No animation available. Perform some operations first.")

            if "RBTree" in action_name:
                # Показываем лексикографический порядок
                st.markdown(
                    "<div style='margin-top: 1rem; padding: 0.75rem; border-radius: 8px; background-color: black; color: white; font-family: monospace; text-align: center; font-size: 1.1rem; letter-spacing: 2px;'>"
                    "а б в г д е ж з и й к л м н о п р с т у ф х ц ч ш щ ъ ы ь э ю я ё"
                    "</div>",
                    unsafe_allow_html=True
                )

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

                is_gen = ("Generate" in action_name) or (action_config.execute_cmd == "37")
                needs_maxflow = action_name == "Min Cost Flow"
                has_local_graph = self._get(f'{lab}_graph_generated', False)
                has_global_graph = self._get('global_graph_generated', False)
                graph_ready = has_local_graph or has_global_graph
                report_file = None
                if action_name == "Fundamental Cuts (all)":
                    report_file = "52_fundamental_cuts.txt"
                elif action_name == "Fundamental Cuts (subset)":
                    report_file = "53_fundamental_cuts_subset.txt"
                disabled = (
                    not is_gen and not graph_ready
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
                            self._set('current_report_file', report_file)
                            if is_gen:
                                self._set(f'{lab}_graph_generated', True)
                                self._set(f'{lab}_maxflow_done', False)
                                if 'Flow Network' not in action_name:
                                    self._set('global_graph_generated', True)
                                self._s.current_visualization = action_config.images
                                if 'Rice' in action_name or action_name == 'fromNetwork':
                                    self._set('dist_type', 'rice')
                                    self._set('dist_source', 'flow' if ('Flow Network' in action_name or action_name == 'fromNetwork') else 'graph')
                                    self._set('dist_a', self._get_param(lab, action_name, 'rayleigh_a', DEFAULT_PARAMS.rayleigh_a))
                                    self._set('dist_h', self._get_param(lab, action_name, 'rayleigh_h', DEFAULT_PARAMS.rayleigh_h))
                                elif 'Acyclic' in action_name or action_name == 'Generate Flow Network':
                                    self._set('dist_type', 'uniform')
                                    self._set('dist_source', 'flow' if 'Flow Network' in action_name else 'graph')
                            if action_name == "Max Flow":
                                self._set(f'{lab}_maxflow_done', True)
                with col2:
                    if st.button("Visualize", disabled=disabled, key=f"{lab}_{action_name}_visualize"):
                        self._set('current_report_file', report_file)
                        self._s.current_visualization = action_config.images

    # ------------------------------------------------------------------
    # Lab UI entry point
    # ------------------------------------------------------------------

    def render_lab_ui(self, lab_name: str) -> None:
        lab_config = LAB_CONFIGS[lab_name]
        with st.sidebar:
            st.header(lab_name)
            if st.button("Back to Main menu", key="back_to_menu"):
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

