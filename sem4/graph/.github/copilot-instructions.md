# Graph Algorithms Labs - AI Coding Guidelines

## Project Overview
This is a modular C++20 project implementing graph algorithms across multiple labs (Shimbell method, flows, combinatorics, cycles, data structures). Core architecture uses `GraphBase<VertexT, EdgeT>` template for all graph types, with Python visualization integration. Labs include: Lab1 (Shimbell method and path counting), Lab3 (flows), Lab4 (combinatorics), Lab5 (cycles), Lab6 (data structures: HashTable, RBTree with raw pointers for memory safety).

## Key Architecture Patterns
- **GraphBase Template**: All graphs inherit from `GraphBase<VertexT, EdgeT>`. Example: `Graph` uses `Vertex` and `EdgeData` (weight), `FlowNetwork` uses `FlowVertex` and `FlowEdge` (capacity/cost/flow).
- **RBTree Structure**: Uses raw pointers (`RBNode*`) for nodes (left/right/parent) to avoid unique_ptr ownership issues. Includes `minimum` (find min in subtree) and `transplant` (replace subtree) for correct removal. Root is `RBNode* m_root`, destroyed recursively in destructor.
- **Entity Classes**: Prefer classes over static functions (e.g., `Generator`, `Visualizer`, `ShimbellMethod`, `RBTree`). Use namespaces like `graph`, `lab1`, `dict`.
- **Data Export for Viz**: C++ exports graph data to `assets/txt/` files, Python scripts in `scripts/visualization/` read and plot using matplotlib/networkx. `plot_network.py` highlights added edges (e.g., for independent sets/covers), `plot_colored_graph.py` colors vertices only (e.g., for vertex sets/colorings), `plot_paths.py` shows paths and added edges. Call via `Visualizer::drawGraph(graph, "assets/png/graph.png")` or `drawRBTree` for trees.

## Visualizer Flags
The `Visualizer` class in `common/` invokes Python scripts with specific command-line arguments. Below are the flags and positional arguments used by each method:

- **drawMatrix**: Positional: `graph_file`, `output_file`, `title`
- **draw (Graph/FlowNetwork)**: Positional: `graph_file`, `output_file`, `directed`, `title`; Flags: `--type` (graph|flow), `--added_edges_file` (optional, path to added edges file)
- **drawPaths**: Positional: `graph_file`, `paths_file`, `output_file`, `directed`, `title`; Flags: `--type` (graph|flow), `--added_edges_file` (optional)
- **drawColoredGraph**: Positional: `graph_file`, `output_file`, `colors_file`, `directed`, `title` (highlights vertices only, no edges)
- **drawHashTable**: Positional: `data_file`, `output_file`, `title`
- **drawRBTree**: Positional: `data_file`, `output_file`, `title` (or `gif_file` for animation)
- **draw (Animation)**: Flags: `--input` (snapshots file), `--output` (gif file), `--graph_type` (directed|undirected), `--show`
- **Unified Menu**: `main.cc` combines all labs using `checkAndRun` with `unique_ptr` graphs. Each lab has a `Runner` class with demo/interactive modes.

## Build & Run Workflows
- **Full Build**: `make` (configures CMake with clang++, builds all libs and executable).
- **Run Menu**: `./build/graph_main` (interactive menu for all labs).
- **Lab-Specific Builds**: `make lab1`, `make lab3`, `make lab4`, `make lab5`, `make lab6` (build individual lab libraries).
- **Notebook Demo**: Run `examples.ipynb` cells to execute C++ via subprocess, generate data/viz, display images. Requires Python venv with `requirements.txt` packages.
- **Code Style**: `make check` runs clang-tidy (read-only), custom `fix-code` target applies clang-tidy fixes + clang-format.

## Coding Conventions
- **C++20 Features**: Use templates, lambdas, `std::optional`, `[[nodiscard]]`, `unique_ptr` (in common/), raw pointers (in RBTree for simplicity).
- **Error Handling**: Use `checkAndRun` for null checks before actions, e.g., `checkAndRun(graph, [&]{ visualize(); }, "Graph not initialized")`.
- **Path Utils**: Use `PathUtils` templates for path operations (e.g., `forEachEdgeInPath`), accepting graph as lambdas for reusability.
- **Random Generation**: `Generator` uses `std::random_device` + `mt19937` for graphs (tree, acyclic, connected).
- **Visualization Calls**: Hardcode venv path in `Utils.h` (e.g., `"/Users/ivan/myvenv/bin/python"`), run scripts as modules: `python -m scripts.visualization.plot_graph`.
- **RBTree Specific**: Use raw pointers for nodes, manual new/delete. Add DEBUG logs in fixup/rotations for tracing. Validate after operations.

## Architectural Preferences
- **DRY (Don't Repeat Yourself)**: Avoid code duplication. Reuse common logic via templates, inheritance (e.g., GraphBase), or utility functions. If code repeats, extract to `common/`.
- **KISS (Keep It Simple, Stupid)**: Prefer simple solutions. Avoid over-engineering — e.g., raw pointers in RBTree for simplicity over unique_ptr complexity.
- **SOLID Principles**:
  - **Single Responsibility**: Each class/method does one thing (e.g., `RBTree` handles tree ops, `Visualizer` handles drawing).
  - **Open-Closed**: Open for extension (via templates like GraphBase), closed for modification.
  - **Liskov Substitution**: Subclasses (e.g., FlowNetwork from GraphBase) should be substitutable.
  - **Interface Segregation**: Keep interfaces minimal (e.g., separate `Visualizer` methods for different types).
  - **Dependency Inversion**: Depend on abstractions (e.g., lambdas in PathUtils), not concretions.
  - **Composition over Inheritance**: Prefer composition (e.g., `Generator` uses random engines) unless inheritance is clearly beneficial (e.g., GraphBase).
  - **Law of Demeter**: Minimize knowledge of other classes' internals. Use public methods to interact (e.g., use `Graph` methods, not internal data structures).
  - **RAII (Resource Acquisition Is Initialization)**: Manage resources via constructors/destructors. Use smart pointers in common/, manual management in RBTree with clear ownership.
  - **YAGNI (You Aren't Gonna Need It)**: Avoid adding features until necessary. E.g., only implement visualization types needed for current labs.
  - **Template Metaprogramming**: Use templates for generic programming (e.g., GraphBase) but avoid excessive complexity that hinders readability.

## Design Patterns
- **Factory Method**: Create objects without specifying the class. *Example*: `Generator` for creating graphs (tree, acyclic, connected).
- **Strategy**: Encapsulate algorithms. *Example*: Different visualization types in `Visualizer` (graph vs tree vs flow).
- **Observer**: Notify changes. *Example*: Potential for logging in RBTree (notify on insert/remove).
- **Decorator**: Add behavior without modifying class. *Example*: Wrappers for graphs (e.g., logging or caching decorator).
- **Command**: Encapsulate requests. *Example*: Menu commands in `main.cc` (run lab1, lab3, etc.).
- **Singleton**: One instance. *Example*: `FileHandler` for centralized I/O.
- **Template Method**: Base class with steps, subclasses override. *Example*: `GraphBase` as template for graph operations.
- **Adapter**: Convert interface. *Example*: Wrappers for legacy code in graphs.
- **Bridge**: Separate abstraction from implementation. *Example*: `Visualizer` separates drawing logic from data.
- **Facade**: Simplify complex subsystems. *Example*: `Visualizer` as facade for Python plots.
- **Mediator**: Centralize communication. *Example*: Menu as mediator between labs.
- **State**: Change behavior with state. *Example*: RBTree balancing states.
- **Visitor**: Add operations to classes. *Example*: Algorithms on graphs without modifying them.

## Integration Points
- **Python Scripts**: Minimal logic, read from `assets/txt/`, output to `assets/png/gif/`. Update both C++ export format and Python parser together.
- **Notebook Automation**: `run_cpp` function feeds inputs to `./build/graph_main`, logs stdout/stderr to `log.txt`, then displays generated images.
- **Asset Management**: Notebook cleans `assets/` at end; C++ overwrites txt files per call.
- **Animation**: For flow networks, `Animator::animateFlowGrowth()` generates GIF animations in `assets/gif/`. Planned for RBTree snapshots.

## Common Pitfalls
- Ensure graph is initialized before lab operations (use `checkAndRun`).
- Update `examples.ipynb` when adding new labs or viz types.
- Match C++ export format to Python script expectations (e.g., edge list with weights for `plot_graph.py`, node-parent-color for `plot_rbtree.py`).
- Lab6 RBTree: Use raw pointers, avoid unique_ptr in nodes. Ensure transplant/minimum are defined before remove. Test validate() after inserts/removes.
- Memory: In RBTree, destroy() recursively deletes nodes; in common/, rely on unique_ptr.
