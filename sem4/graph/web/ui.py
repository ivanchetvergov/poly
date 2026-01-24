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

    # 2x3 grid for labs
    labs = ["Lab 1: Shimbell Method and Path Counting", "Lab 2: Placeholder", "Lab 3: Flows (Max Flow, Min Cost Flow)",
            "Lab 4: Graph Combinatorics", "Lab 5: Cycles (Eulerian, Hamiltonian, TSP)", "Lab 6: Data Structures (HashTable, RBTree)"]

    col1, col2, col3 = st.columns(3)
    cols = [col1, col2, col3]

    for i, lab_name in enumerate(labs):
        with cols[i % 3]:
            if lab_name == "Lab 2: Placeholder":
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
                        st.rerun()

def render_lab_ui(lab_name):
    lab_config = LAB_CONFIGS[lab_name]

    with st.sidebar:
        st.header(f"{lab_name}")

        col1, col2 = st.columns(2)
        with col1:
            if st.button("Back to menu", key="back_to_menu"):
                send_command(lab_name, "0")
                time.sleep(0.5)
                kill_process(lab_name)
                st.session_state.current_lab = None
                st.rerun()
        with col2:
            if st.button("Clear Output", key=f"{lab_name}_clear_output"):
                st.session_state.processes[lab_name]['output'] = ""
                st.session_state.current_visualization = []
                # Clear assets
                for dir_name in ["png", "gif"]:
                    dir_path = ASSETS_DIR / dir_name
                    if dir_path.exists():
                        shutil.rmtree(dir_path)
                        dir_path.mkdir()
                st.rerun()

        if not is_process_running(lab_name):
            st.error("Process not running. Go back and restart.")
            return

        # Actions as expanders
        vertices = st.session_state.get(f"{lab_name}_current_vertices", DEFAULT_PARAMS.vertices)
        for action_name, action_config in lab_config.sub_actions.items():
            with st.expander(action_name):
                for param in action_config.params or []:
                    if param == "directed":
                        st.checkbox("Directed", value=DEFAULT_PARAMS.directed, key=f"{lab_name}_{action_name}_directed")
                    elif param == "vertices":
                        st.number_input("Vertices", 3, 50, DEFAULT_PARAMS.vertices, key=f"{lab_name}_{action_name}_vertices")
                    elif param == "edges":
                        st.number_input("Edges", 5, 100, DEFAULT_PARAMS.edges, key=f"{lab_name}_{action_name}_edges")
                    elif param == "start_vertex":
                        st.number_input("Start Vertex", 0, vertices-1, DEFAULT_PARAMS.start_vertex, key=f"{lab_name}_{action_name}_start_v")
                    elif param == "end_vertex":
                        st.number_input("End Vertex", 0, vertices-1, DEFAULT_PARAMS.end_vertex, key=f"{lab_name}_{action_name}_end_v")
                    elif param == "source":
                        st.number_input("Source", 0, vertices-1, DEFAULT_PARAMS.source, key=f"{lab_name}_{action_name}_source")
                    elif param == "sink":
                        st.number_input("Sink", 0, vertices-1, DEFAULT_PARAMS.sink, key=f"{lab_name}_{action_name}_sink")
                    elif param == "distance":
                        st.number_input("Distance", 1, vertices-1, DEFAULT_PARAMS.distance, key=f"{lab_name}_{action_name}_distance")

                col1, col2 = st.columns(2)
                with col1:
                    if st.button("Execute", key=f"{lab_name}_{action_name}_execute"):
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
                            send_command(lab_name, f"{cmd}{params_str}")
                            time.sleep(1.0)

                with col2:
                    if st.button("Visualize", key=f"{lab_name}_{action_name}_visualize"):
                        st.session_state.current_visualization = action_config.images

    # Current Visualization
    if st.session_state.current_visualization:
        if len(st.session_state.current_visualization) == 2:
            col1, col2 = st.columns(2)
            with col1:
                img = st.session_state.current_visualization[0]
                img_path = ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img
                if img_path.exists() and os.path.getsize(str(img_path)) > 0:
                    try:
                        with Image.open(str(img_path)) as im:
                            im.verify()
                        st.image(str(img_path))
                    except Exception as e:
                        pass
            with col2:
                img = st.session_state.current_visualization[1]
                img_path = ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img
                if img_path.exists() and os.path.getsize(str(img_path)) > 0:
                    try:
                        with Image.open(str(img_path)) as im:
                            im.verify()
                        st.image(str(img_path))
                    except Exception as e:
                        pass
        else:
            for img in st.session_state.current_visualization:
                img_path = ASSETS_DIR / ("gif" if img.endswith('.gif') else "png") / img
                if img_path.exists() and os.path.getsize(str(img_path)) > 0:
                    try:
                        with Image.open(str(img_path)) as im:
                            im.verify()
                        st.image(str(img_path))
                    except Exception as e:
                        pass
