import streamlit as st
from ui import render_main_menu, render_lab_ui

def main():
    st.set_page_config(page_title="Graph Labs Web Interface", layout="wide")

    # Initialize session state
    if 'processes' not in st.session_state:
        st.session_state.processes = {}
    if 'current_lab' not in st.session_state:
        st.session_state.current_lab = None
    if 'current_visualization' not in st.session_state:
        st.session_state.current_visualization = []

    # Add CSS for rounded buttons and disabled styles
    st.markdown("""
    <style>
    .main-menu-button button {
        border-radius: 15px;
        font-size: 32px;
        padding: 30px 60px;
        margin: 10px;
        width: 100%;
        height: 120px;
    }
    .stButton > button {
        border-radius: 15px;
        font-size: 18px;
        padding: 10px 20px;
    }
    .stButton {
        display: flex;
        justify-content: center;
    }
    .st-emotion-cache-1r6slb0 {
        opacity: 0.5;
    }
    img {
        max-width: 100%;
        max-height: 80vh;
        height: auto;
        display: block;
        margin: 0 auto;
    }
    </style>
    """, unsafe_allow_html=True)

    st.markdown("<h1 style='margin-top: -60px;'>Graph Algorithms Labs - Interactive Web Interface</h1>", unsafe_allow_html=True)

    if st.session_state.current_lab is None:
        render_main_menu()
    else:
        render_lab_ui(st.session_state.current_lab)

if __name__ == "__main__":
    main()
