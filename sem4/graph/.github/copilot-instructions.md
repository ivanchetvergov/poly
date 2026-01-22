# Graph Algorithms Labs - AI Coding Guidelines

## Project Overview
This is a modular C++20 project implementing graph algorithms across multiple labs (Shimbell method, flows, combinatorics, cycles, data structures). Core architecture uses `GraphBase<VertexT, EdgeT>` template for all graph types, with Python visualization integration. Labs include: Lab1 (Shimbell method and path counting), Lab3 (flows), Lab4 (combinatorics), Lab5 (cycles), Lab6 (data structures: HashTable, RBTree).

## Key Architecture Patterns
- **GraphBase Template**: All graphs inherit from `GraphBase<VertexT, EdgeT>`. Example: `Graph` uses `Vertex` and `EdgeData` (weight), `FlowNetwork` uses `FlowVertex` and `FlowEdge` (capacity/cost/flow).
- **Entity Classes**: Prefer classes over static functions (e.g., `Generator`, `Visualizer`, `ShimbellMethod`). Use namespaces like `graph`, `lab1`.
- **Data Export for Viz**: C++ exports graph data to `assets/txt/` files, Python scripts in `scripts/visualization/` read and plot using matplotlib/networkx. Call via `Visualizer::drawGraph(graph, "assets/png/graph.png")`.
- **Unified Menu**: `main.cc` combines all labs using `checkAndRun` with `unique_ptr` graphs. Each lab has a `Runner` class.

## Build & Run Workflows
- **Full Build**: `make` (configures CMake with clang++, builds all libs and executable).
- **Run Menu**: `./build/graph_main` (interactive menu for all labs).
- **Lab-Specific Builds**: `make lab1`, `make lab3`, `make lab4`, `make lab5` (build individual lab libraries).
- **Notebook Demo**: Run `examples.ipynb` cells to execute C++ via subprocess, generate data/viz, display images. Requires Python venv with `requirements.txt` packages.
- **Code Style**: `make check` runs clang-tidy (read-only), custom `fix-code` target applies clang-tidy fixes + clang-format.

## Coding Conventions
- **C++20 Features**: Use templates, lambdas, `std::optional`, `[[nodiscard]]`, `unique_ptr`. Avoid raw pointers.
- **Error Handling**: Use `checkAndRun` for null checks before actions, e.g., `checkAndRun(graph, [&]{ visualize(); }, "Graph not initialized")`.
- **Path Utils**: Use `PathUtils` templates for path operations (e.g., `forEachEdgeInPath`), accepting graph as lambdas for reusability.
- **Random Generation**: `Generator` uses `std::random_device` + `mt19937` for graphs (tree, acyclic, connected).
- **Visualization Calls**: Hardcode venv path in `Utils.h` (e.g., `"/Users/ivan/myvenv/bin/python"`), run scripts as modules: `python -m scripts.visualization.plot_graph`.

## Integration Points
- **Python Scripts**: Minimal logic, read from `assets/txt/`, output to `assets/png/gif/`. Update both C++ export format and Python parser together.
- **Notebook Automation**: `run_cpp` function feeds inputs to `./build/graph_main`, logs stdout/stderr to `log.txt`, then displays generated images.
- **Asset Management**: Notebook cleans `assets/` at end; C++ overwrites txt files per call.
- **Animation**: For flow networks, `Animator::animateFlowGrowth()` generates GIF animations in `assets/gif/`.

## Common Pitfalls
- Ensure graph is initialized before lab operations (use `checkAndRun`).
- Update `examples.ipynb` when adding new labs or viz types.
- Match C++ export format to Python script expectations (e.g., edge list with weights for `plot_graph.py`).
- Lab6 is standalone (data structures) and not integrated into the main menu.
