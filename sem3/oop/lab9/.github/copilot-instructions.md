# Copilot / AI agent instructions for sem-3 (lab9)

Goal: help an AI coding agent become productive quickly in this Qt/C++ codebase.

Key concepts (big picture)
- Architecture: three-layered separation: `domain/` (model, Shape, ShapeData, ConnectionItem),
  `infrastructure/` (DB access — `DatabaseManager`, `ShapeRepository`), and
  `presentation/` (Qt UI — `GraphicsView`, `MainWindow`, delegates, models).
- Data flow: DB <-> `ShapeRepository` <-> `GraphicsView`/presentation. `ShapeRepository` exposes
  Boost.Signals2 signals (`shapeAdded`, `shapeRemoved`, `shapeUpdated`) that the view subscribes to.
- Ownership: presentation elements are `QGraphicsItem`-derived; the scene stores items (use
  `m_scene->addItem(shape)` — scene becomes owner). Domain `Shape` is a `QGraphicsObject`
  and uses composition: child `QGraphicsEllipseItem`/`QGraphicsRectItem`/`QGraphicsPolygonItem`
  are created as delegates and parented via `setParentItem(this)`.

Important files to inspect
- `include/domain/shape.h`, `src/domain/shape.cc` — shape model, drawing delegation, `applyData()`.
- `include/infrastructure/shape_repository.h`, `src/infrastructure/shape_repository.cc` — DB CRUD,
  signals and connection management.
- `include/presentation/graphics_view.h`, `src/presentation/graphics_view.cc` — scene lifecycle,
  subscription to repository signals, user interaction (mouse events), and connection items.
- `src/domain/shape_factory.cc` — maps `ShapeData` to concrete domain objects.
- `CMakeLists.txt` — Qt6 + Boost settings, build flags and helpful options such as `ENABLE_ASAN`.

Common patterns and gotchas (project-specific)
- Use composition, not multiple inheritance, for Qt graphics items: do NOT inherit both
  `QGraphicsObject` and a `QGraphicsEllipseItem` (that causes ambiguous `QGraphicsItem` bases).
  Instead `Shape` is `QGraphicsObject` and it creates a child `QGraphicsEllipseItem` delegate.
- Repository signals: `ShapeRepository` uses Boost.Signals2 (not Qt signals) for cross-layer
  notifications — code in presentation subscribes using `m_repository->shapeAdded.connect(...)`.
- `applyData(const ShapeData&)`: implement this on `Shape` subclasses to update geometry (rect/polygon),
  pen/brush, visibility — and perform an EPS comparison before calling `setPos()` to avoid tiny
  numeric updates that trigger `itemChange` cascades.
- Avoid full-object `update()` on small changes: use `updatePosition()` in `ShapeRepository` when only
  the position changed (this reduces side-effects on other fields and avoids unnecessary reloads).
- Guard against update echo loops: when the view writes a change to the repository, the repository will
  emit `shapeUpdated`; the view should ignore the immediate echo for that id (or use a short-lived
  guard set or debounce timer). Prefer a short EPS check in `applyData` too.

Build / run / debug
- Typical build workflow (macOS / zsh):
```bash
rm -rf build && mkdir build && cd build
cmake ..
make -j4
./lab9
```
- Useful CMake options in repository root:
  - `cmake -DENABLE_ASAN=ON ..` to enable AddressSanitizer (if using Clang/GCC).
- If you see a CMake warning about Boost (CMP0167), `CMakeLists.txt` should set the policy; prefer
  leaving it set in the file rather than silencing warning in the agent's patch unless necessary.
- When debugging runtime behavior (position jitter, ownership issues):
  - Add `qDebug()` temporarily in `GraphicsView::mouseReleaseEvent`, `ShapeRepository::updatePosition`,
    and `GraphicsView::onShapeUpdated` to log old/new positions and event ordering.
  - Use `lldb` or Qt Creator to step through `itemChange` and `applyData`.

Code-change guidance for AI agents
- Keep diffs minimal and focused: change the smallest number of files required.
- Preserve ownership semantics: when adding items to the scene, make sure `scene->addItem()` is called
  and that child delegates are parented with `setParentItem(this)` to avoid double-free or missing owners.
- Use `qgraphicsitem_cast<T*>(QGraphicsItem*)` to downcast `QGraphicsItem*` to item-type delegates.
- When adding repository signals, follow the existing Boost.Signals2 pattern (declare in header, emit in
  implementation after successful DB operations).
- Avoid reintroducing multiple inheritance between `QGraphicsObject` and other `QGraphicsItem` derivatives.

Examples (patterns in this codebase)
- Update position on drag release (current pattern):
  - `GraphicsView::mouseReleaseEvent()` computes `shapeData.position = shape->scenePos()` and calls
    `m_repository->updatePosition(id, pos)` (preferred) or `update()` (full update).
- Repository -> View notification:
  - `ShapeRepository::updatePosition` calls `shapeUpdated(id)`; `GraphicsView` subscribes and uses
    `onShapeUpdated(id)` to call `shape->applyData()` or recreate the shape if it doesn't exist.

What agents should NOT do
- Do not change the high-level data flow (DB -> repository -> presentation) without explicit approval.
- Do not mix Boost.Signal usage with Qt signals in new code — follow existing layers (Boost in infra,
  Qt signals for domain/presentation local events).
- Avoid changing the scene ownership model (scene owns top-level `Shape` objects). If ownership must
  change, update all creation/deletion paths and tests.

If anything is unclear or you'd like me to add examples for a specific task (e.g., implement live-saving
with debounce, or add a unit-test harness), tell me which area and I'll iterate on this file.
