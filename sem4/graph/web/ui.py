import streamlit as st
import subprocess
import time
import os
import shutil
from pathlib import Path
from PIL import Image
from config import LAB_CONFIGS, DEFAULT_PARAMS

PROJECT_ROOT = Path(__file__).parent.parent
BUILD_DIR = PROJECT_ROOT / "build"
ASSETS_DIR = PROJECT_ROOT / "assets"
EXECUTABLE = BUILD_DIR / "graph_main"

def send_command(lab_name, command):
    """Send a command to the process."""
    print(f"DEBUG: Sending command: {command}")
    if lab_name not in st.session_state.processes:
        return
    proc = st.session_state.processes[lab_name]['proc']
    if proc.poll() is None:
        proc.stdin.write(command + '\n')
        proc.stdin.flush()

def kill_process(lab_name):
    """Kill the process for the lab."""
    if lab_name in st.session_state.processes:
        proc = st.session_state.processes[lab_name]['proc']
        if proc.poll() is None:
            proc.terminate()
            proc.wait()
        del st.session_state.processes[lab_name]

def is_process_running(lab_name):
    """Check if process is running."""
    if lab_name in st.session_state.processes:
        proc = st.session_state.processes[lab_name]['proc']
        return proc.poll() is None
    return False

def start_process(lab_name):
    """Start a new C++ process for the lab."""
    if lab_name in st.session_state.processes:
        return  # Already running

    proc = subprocess.Popen(
        [str(EXECUTABLE)],
        stdin=subprocess.PIPE,
        text=True,
        cwd=PROJECT_ROOT
    )
    st.session_state.processes[lab_name] = {
        'proc': proc,
        'output': '',
        'start_time': time.time()
    }

def render_main_menu():
    st.markdown("<h2 style='text-align: center;'>Select a Lab</h2>", unsafe_allow_html=True)
    st.markdown('<style>.stButton button { margin: 0 !important; padding: 2rem 4rem !important; font-size: 5rem !important; height: 7.2rem !important; width: 500px !important; }</style>', unsafe_allow_html=True)
    st.markdown('<div style="display: flex; justify-content: center;"><div style="max-width: 800px;">', unsafe_allow_html=True)

    # 2x3 grid for labs
    labs = ["Lab 1: Shimbell Method and Path Counting", "Lab 2: Traversals and Shortest Paths", "Lab 3: Flows (Max Flow, Min Cost Flow)",
            "Lab 4: Graph Combinatorics", "Lab 5: Cycles (Eulerian, Hamiltonian, TSP)", "Lab 6: Data Structures (HashTable, RBTree)"]

    disabled_labs = ["Lab 2: Traversals and Shortest Paths", "Lab 4: Graph Combinatorics", "Lab 5: Cycles (Eulerian, Hamiltonian, TSP)"]

    col1, col2, col3 = st.columns(3, gap="xxsmall")
    cols = [col1, col2, col3]

    for i, lab_name in enumerate(labs):
        with cols[i % 3]:
            # if lab_name in disabled_labs:
            if lab_name == "Lab 2: Traversals and Shortest Paths":
                st.button(lab_name, disabled=True, key=f"select_{lab_name}")
            else:
                running = is_process_running(lab_name)
                if running:
                    st.error(f"{lab_name} (Running)")
                    if st.button(f"Stop {lab_name}", key=f"stop_{lab_name}"):
                        kill_process(lab_name)
                        st.rerun()
                else:
                    if st.button(lab_name, key=f"select_{lab_name}"):
                        start_process(lab_name)
                        st.session_state.current_lab = lab_name
                        st.session_state.graph_generated = False
                        st.session_state.current_visualization = []
                        # Clear assets
                        for dir_name in ["png", "gif"]:
                            dir_path = ASSETS_DIR / dir_name
                            if dir_path.exists():
                                shutil.rmtree(dir_path)
                                dir_path.mkdir()
                        st.rerun()

    st.markdown('</div></div>', unsafe_allow_html=True)

def display_image(img_path):
    if img_path.exists() and os.path.getsize(str(img_path)) > 0:
        try:
            with Image.open(str(img_path)) as im:
                im.verify()
            if str(img_path).endswith('.gif'):
                import base64
                with open(str(img_path), "rb") as f:
                    data = base64.b64encode(f.read()).decode()
                st.markdown(f'<img src="data:image/gif;base64,{data}" style="width:100%;">', unsafe_allow_html=True)
            else:
                st.image(str(img_path))
        except Exception as e:
            pass

def render_lab_ui(lab_name):
    lab_config = LAB_CONFIGS[lab_name]

    with st.sidebar:
        st.header(f"{lab_name}")

        if st.button("Back to Main menu", key="back_to_menu"):
            send_command(lab_name, "0")
            time.sleep(0.5)
            kill_process(lab_name)
            st.session_state.current_lab = None
            st.rerun()

        if not is_process_running(lab_name):
            st.error("Process not running. Go back and restart.")
            return

        # Actions as expanders or toggle for Lab 6
        current_vertices = st.session_state.get(f"{lab_name}_current_vertices", DEFAULT_PARAMS.vertices)
        current_edges = st.session_state.get(f"{lab_name}_current_edges", DEFAULT_PARAMS.edges)
        if lab_name == "Lab 6: Data Structures (HashTable, RBTree)":
            is_any_interactive_open = any(st.session_state.get(f"{lab_name}_{act}_open", False) for act in lab_config.sub_actions if "Interactive" in act)
            for action_name, action_config in lab_config.sub_actions.items():
                is_interactive = "Interactive" in action_name
                open_key = f"{lab_name}_{action_name}_open"
                if is_interactive:
                    disabled_open = is_any_interactive_open and not st.session_state.get(open_key, False)
                    if st.button(f"Open {action_name}", key=f"open_{action_name}", disabled=disabled_open):
                        st.session_state[open_key] = True
                    if st.session_state.get(open_key, False):
                        entered_key = f"{open_key}_entered"
                        if not st.session_state.get(entered_key, False):
                            send_command(lab_name, str(action_config.execute_cmd))
                            st.session_state[entered_key] = True
                        with st.container():
                            with st.form(key=f"{lab_name}_{action_name}_form"):
                                operation = st.selectbox("Operation", ["insert", "remove", "search"], key=f"{lab_name}_{action_name}_operation")
                                word = st.text_input("Word", key=f"{lab_name}_{action_name}_word")
                                submitted = st.form_submit_button("Submit Command")
                                if submitted:
                                    op = operation
                                    params_str = f"{op} {word}"
                                    send_command(lab_name, params_str)
                                    time.sleep(3.0)
                            if st.button("Exit from interactive loop", key=f"{lab_name}_{action_name}_exit"):
                                send_command(lab_name, "exit")
                                st.session_state[open_key] = False
                                st.session_state[entered_key] = False
                                time.sleep(0.5)
                                st.rerun()
                            col1, col2 = st.columns(2, gap="xxsmall")
                            with col1:
                                if st.button("Visualize", key=f"{lab_name}_{action_name}_draw"):
                                    send_command(lab_name, "draw")
                                    st.session_state.current_visualization = action_config.images
                            with col2:
                                if "RBTree" in action_name:
                                    if st.button("GIF", key=f"{lab_name}_{action_name}_gif"):
                                        send_command(lab_name, "gif")
                                        st.session_state.current_visualization = ["65_rbtree_growth.gif"]
                else:
                    if is_any_interactive_open:
                        st.write(f"{action_name} (disabled while interactive mode is open)")
                    else:
                        with st.expander(action_name):
                            col1, col2 = st.columns(2, gap="xxsmall")
                            with col1:
                                disabled_exec = False
                                if st.button("Execute", disabled=disabled_exec, key=f"{lab_name}_{action_name}_execute"):
                                    with st.spinner("Executing..."):
                                        cmd = action_config.execute_cmd
                                        send_command(lab_name, cmd)
                                        time.sleep(2.0)
                            with col2:
                                disabled_vis = False
                                if st.button("Visualize", disabled=disabled_vis, key=f"{lab_name}_{action_name}_visualize"):
                                    st.session_state.current_visualization = action_config.images
        else:
            for action_name, action_config in lab_config.sub_actions.items():
                with st.expander(action_name):
                    for param in action_config.params or []:
                        if param == "directed":
                            st.checkbox("Directed", value=DEFAULT_PARAMS.directed, key=f"{lab_name}_{action_name}_directed")
                        elif param == "vertices":
                            local_vertices = st.session_state.get(f"{lab_name}_{action_name}_vertices", DEFAULT_PARAMS.vertices)
                            st.number_input("Vertices", 3, 50, DEFAULT_PARAMS.vertices, key=f"{lab_name}_{action_name}_vertices")
                            st.session_state[f"{lab_name}_current_vertices"] = local_vertices
                        elif param == "edges":
                            st.number_input("Edges", 5, 100, DEFAULT_PARAMS.edges, key=f"{lab_name}_{action_name}_edges")
                        elif param == "start_vertex":
                            st.number_input("Start Vertex", 0, current_vertices-1, min(DEFAULT_PARAMS.start_vertex, current_vertices-1), key=f"{lab_name}_{action_name}_start_v")
                        elif param == "end_vertex":
                            st.number_input("End Vertex", 0, current_vertices-1, min(DEFAULT_PARAMS.end_vertex, current_vertices-1), key=f"{lab_name}_{action_name}_end_v")
                        elif param == "source":
                            st.number_input("Source", 0, current_vertices-1, min(DEFAULT_PARAMS.source, current_vertices-1), key=f"{lab_name}_{action_name}_source")
                        elif param == "sink":
                            st.number_input("Sink", 0, current_vertices-1, min(DEFAULT_PARAMS.sink, current_vertices-1), key=f"{lab_name}_{action_name}_sink")
                        elif param == "distance":
                            st.number_input("Distance", 1, current_edges, min(DEFAULT_PARAMS.distance, current_edges), key=f"{lab_name}_{action_name}_distance")
                        elif param == "operation":
                            st.selectbox("Operation", ["insert", "delete", "search"], key=f"{lab_name}_{action_name}_operation")
                        elif param == "word":
                            st.text_input("Word", key=f"{lab_name}_{action_name}_word")

                    col1, col2 = st.columns(2, gap="xxsmall")
                    with col1:
                        disabled_exec = ("Generate" not in action_name and not st.session_state.get('graph_generated', False)) and lab_name != "Lab 6: Data Structures (HashTable, RBTree)"
                        if st.button("Execute", disabled=disabled_exec, key=f"{lab_name}_{action_name}_execute"):
                            with st.spinner("Executing..."):
                                cmd = action_config.execute_cmd
                                params_str = ""
                                if action_config.params:
                                    if "directed" in action_config.params:
                                        directed = st.session_state.get(f"{lab_name}_{action_name}_directed", DEFAULT_PARAMS.directed)
                                        params_str += f"\n{'1' if directed else '0'}"
                                    if "vertices" in action_config.params:
                                        vertices = st.session_state.get(f"{lab_name}_{action_name}_vertices", DEFAULT_PARAMS.vertices)
                                        params_str += f"\n{vertices}"
                                        st.session_state[f"{lab_name}_current_vertices"] = vertices
                                    if "edges" in action_config.params:
                                        edges = st.session_state.get(f"{lab_name}_{action_name}_edges", DEFAULT_PARAMS.edges)
                                        params_str += f"\n{edges}"
                                        st.session_state[f"{lab_name}_current_edges"] = edges
                                    if "start_vertex" in action_config.params:
                                        start_v = st.session_state.get(f"{lab_name}_{action_name}_start_v", DEFAULT_PARAMS.start_vertex)
                                        params_str += f"\n{start_v}"
                                    if "end_vertex" in action_config.params:
                                        end_v = st.session_state.get(f"{lab_name}_{action_name}_end_v", DEFAULT_PARAMS.end_vertex)
                                        params_str += f"\n{end_v}"
                                    if "source" in action_config.params:
                                        source = st.session_state.get(f"{lab_name}_{action_name}_source", DEFAULT_PARAMS.source)
                                        params_str += f"\n{source}"
                                    if "sink" in action_config.params:
                                        sink = st.session_state.get(f"{lab_name}_{action_name}_sink", DEFAULT_PARAMS.sink)
                                        params_str += f"\n{sink}"
                                        if action_name == "Min Cost Flow":
                                            params_str += "\n1"
                                    if "operation" in action_config.params:
                                        operation = st.session_state.get(f"{lab_name}_{action_name}_operation", "insert")
                                        params_str += f"\n{operation}"
                                    if "word" in action_config.params:
                                        word = st.session_state.get(f"{lab_name}_{action_name}_word", "")
                                        params_str += f"\n{word}"
                                send_command(lab_name, f"{cmd}{params_str}")
                                if "Generate" in action_name:
                                    st.session_state.graph_generated = True
                                time.sleep(2.0)

                    with col2:
                        disabled_vis = ("Generate" not in action_name and not st.session_state.get('graph_generated', False)) and lab_name != "Lab 6: Data Structures (HashTable, RBTree)"
                        if st.button("Visualize", disabled=disabled_vis, key=f"{lab_name}_{action_name}_visualize"):
                            st.session_state.current_visualization = action_config.images

    # Current Visualization
    if st.session_state.current_visualization:
        if len(st.session_state.current_visualization) == 2:
            col1, col2 = st.columns(2)
            with col1:
                img = st.session_state.current_visualization[0]
                img_path = ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img
                display_image(img_path)
            with col2:
                img = st.session_state.current_visualization[1]
                img_path = ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img
                display_image(img_path)
        else:
            for img in st.session_state.current_visualization:
                img_path = ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img
                display_image(img_path)
