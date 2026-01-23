# Graph Labs

Лабы по теории графов на C++20. Единое меню, визуализация через Python, модульная архитектура.

## Структура проекта

```bash
graph/
├── CMakeLists.txt            # Корневой CMake файл
├── Makefile                  # Makefile для сборки
├── main.cc                   # Единое меню для всех лаб
├── examples.ipynb            # Ноутбук с интеграцией
├── requirements.txt          # Python зависимости
├── TODO                      # Список задач
├── log.txt                   # Логи от C++ программы (генерируется)
├── common/                   # Переиспользуемый код
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── build/                # Сборка common
│   ├── include/
│   │   ├── CollectionUtils.h # Хелперы для векторов/мапов
│   │   ├── DrawDataConfig.h  # Конфиги для визуализации (пути к файлам)
│   │   ├── FileHandler.h     # Работа с файлами
│   │   ├── Generator.h       # Генератор графов (дерево, ацикл, связный)
│   │   ├── Graph.h           # Обычный граф (взвешенный, ор/неор)
│   │   ├── GraphBase.h       # Template базовый класс для всех графов
│   │   ├── Menu.h            # Класс меню для выбора пунктов
│   │   ├── PathUtils.h       # Template-утилиты для путей (поиск мин, apply)
│   │   ├── Utils.h           # Меню, чтение инпута, checkAndRun
│   │   └── Visualizer.h      # Врапер для вызова пайтон скриптов
│   └── src/
│       ├── DrawDataConfig.cc
│       ├── FileHandler.cc
│       ├── Generator.cc
│       ├── Generator.tpp
│       ├── Graph.cc
│       ├── GraphBase.tpp
│       ├── Menu.cc
│       ├── Utils.cc
│       └── Visualizer.cc
├── examples_files/         # Примеры файлов
├── lab1/                   # Метод Шимбелла и подсчет путей
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── build/
│   ├── include/
│   │   ├── PathCounter.h
│   │   ├── Runner.h
│   │   └── ShimbellMethod.h
│   └── src/
│       ├── PathCounter.cc
│       ├── Runner.cc
│       └── ShimbellMethod.cc
├── lab2/                   # Обходы и кратчайшие пути
│   ├── CMakeLists.txt
│   ├── main.cc
│   ├── README.md
│   ├── include/
│   │   ├── Comparator.h
│   │   └── Runner.h
│   └── src/
│       ├── Comparator.cc
│       └── Runner.cc
├── lab3/                   # Потоки (Форд-Фалкерсон, минимальная стоимость)
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── build/
│   ├── include/
│   │   ├── FlowNetwork.h
│   │   ├── MaxFlow.h
│   │   ├── MinCostFlow.h
│   │   └── Runner.h
│   └── src/
│       ├── FlowNetwork.cc
│       ├── MaxFlow.cc
│       ├── MinCostFlow.cc
│       └── Runner.cc
├── lab4/                   # Остовы и комбинаторика
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── include/
│   └── src/
├── lab5/                   # Циклы (Эйлер, Гамильтон, TSP)
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── build/
│   ├── include/
│   └── src/
├── lab6/                   # Структуры данных (хеш-таблица, RB-дерево)
│   ├── CMakeLists.txt
│   ├── main.cc
│   ├── Makefile
│   ├── README.md
│   ├── build/
│   ├── include/
│   └── src/
│   └── tests/
├── scripts/                # Python для визуализации и утилит
│   ├── README.md
│   ├── __pycache__/
│   ├── core/               # Ядро (конфиги, загрузка, рендеринг)
│   │   ├── __init__.py
│   │   ├── config.py       # Конфигурация цветов, размеров, колормапов
│   │   ├── graph_loader.py # Загрузка графов/цветов/путей из txt-файлов
│   │   ├── helpers.py      # Вспомогательные функции (пути, нормализация)
│   │   └── renderer.py     # Класс Renderer для рисования через matplotlib
│   └── visualization/      # Визуализация
│       ├── plot_colored_graph.py     # Раскраска графов (вершины)
│       ├── plot_flow_animation.py    # Анимация потоков
│       ├── plot_hashtable.py         # Хешмапа через FancyBboxPatch
│       ├── plot_matrix.py            # Матрицы через seaborn
│       ├── plot_network.py           # Графы/сети
│       ├── plot_paths.py             # Графы/сети с выделынными путями
        ├── plot_rbtree_animation.py  # Анимация степов построения КБ Дерева
        └── plot_rbtree               # Визуализация КБ дерева(статика)
├── build/                  # Сборка всего проекта
│   ├── cmake_install.cmake
│   ├── CMakeCache.txt
│   ├── compile_commands.json
│   ├── graph_main
│   ├── CMakeFiles/
│   ├── common/
│   ├── lab1/
│   ├── lab3/
│   ├── lab5/
│   └── lab6/
└── assets/
    ├── png/                # Готовые картинки
    ├── txt/                # Промежуточные данные для Python
    └── gif/                # Анимации (потоки)
```

## Архитектура

### GraphBase<VertexT, EdgeT>

Базовый template-класс для всех графов. Параметризуется типом вершины и ребра.

```cpp
template<typename VertexT, typename EdgeT>
class GraphBase {
    bool isDirected_;
    unordered_map<int, unique_ptr<VertexT>> m_vertices_;
    unordered_map<string, EdgeT> m_edges_;  // ключ = makeKey(from, to)
};
```

### Интеграция с Python

C++ экспортирует данные в txt-файлы, Python скрипты читают их и рисуют через matplotlib/networkx. Ноутбук запускает C++ через subprocess, потом показывает картинки. Логи в log.txt для дебага.

## Сборка и запуск

### Полная сборка

```bash
make  # Конфигурирует CMake с clang++, собирает все библиотеки и исполняемый файл
```

### Запуск меню

```bash
./build/graph_main  # Интерактивное меню для всех лаб
```

### Сборка отдельных лаб

```bash
make lab1  # Только lab1
make lab3  # Только lab3
make lab4  # Только lab4
make lab5  # Только lab5
make lab6  # Только lab6
```

### Проверка кода

```bash
make check     # Запускает clang-tidy (только чтение)
make fix-code  # Применяет clang-tidy fixes + clang-format
```

### Запуск через нотбук

1. Установи зависимости: `pip install -r requirements.txt` (jupyter, matplotlib, networkx, seaborn)
2. Запусти Jupyter: `jupyter notebook` или `jupyter lab`
3. Открой `examples.ipynb`
4. Запусти все ячейки — оно само сгенерит данные, визуализации и очистит assets в конце.

Ноутбук покрывает все лабы: генерит графы, считает Шимбелла/потоки/циклы, рисует матрицы/графы/анимации. Логи в `log.txt`, картинки в `assets/`.

## Технологии

- **C++20**: STL, templates, smart pointers.
- **Python**: matplotlib, networkx, seaborn для визуализации.
- **CMake**: Сборка проекта.
- **Jupyter**: Интерактивный ноутбук для демо.

## Технические детали

### Базовый класс GraphBase

Базовый template-класс для всех графов. Параметризуется типом вершины и ребра.

```cpp
template<typename VertexT, typename EdgeT>
class GraphBase {
    bool isDirected_;
    unordered_map<int, unique_ptr<VertexT>> m_vertices_;
    unordered_map<string, EdgeT> m_edges_;  // ключ = makeKey(from, to)
};
```

Идея: один базовый класс, разные специализации. `Graph` использует `Vertex` и `Edge` (просто вес), `FlowNetwork` использует `FlowVertex` и `FlowEdge` (capacity, cost, flow).

### Generator

Умеет генерить разные типы графов:

- `generateTree` — остовное дерево (n-1 ребер)
- `generateAcyclicGraph` — ацикличный граф с топосортом
- `generateConnectedGraph` — связный граф (сначала дерево, потом добавляет рандомные ребра)

Использует внутри `std::random_device` и `mt19937` для нормальной рандомности.

### PathUtils

Template-функции для работы с путями. Принимают граф как набор колбэков.

```cpp
template<typename Func>
static void forEachEdgeInPath(
    int source,
    int sink,
    const std::unordered_map<int, int>& parent,
    Func func)
{
    for (int v = sink; v != source; v = parent.at(v)) {
        int u = parent.at(v);
        func(u, v);
    }
}
...
```

Идея: не привязываться к конкретному графу, работать через лямбды. Можно переиспользовать в разных алгоритмах.

### Visualizer

Экспортирует граф в txt-файл, потом вызывает питоновский скрипт через `system()`.

Почему так: C++ не умеет в нормальные графики, Python с matplotlib/networkx делает это за секунду. Разделение ответственности.

```cpp
Visualizer::drawGraph(graph, "assets/png/graph.png");
// 1. Экспорт в assets/txt/graph.txt
// 2. system("python3 scripts/plot_graph.py assets/txt/graph.txt assets/png/graph.png")
```

Для каждого типа визуализации свой скрипт и формат txt-файла.

## Лабы

### Lab 1: Метод Шимбелла и подсчет путей

Ищет кратчайшие/длиннейшие пути заданной длины через матричное умножение. Алгоритм: вместо обычного умножения матриц делаем `A^k[i][j] = min(A^(k-1)[i][t] + A[t][j])`.

- `ShimbellMethod` — template-класс, параметризуется операцией (min/max)
- `PathCounter` — рекурсивный DFS с мемоизацией для подсчета всех маршрутов

### Lab 3: Потоки в сетях

Форд-Фалкерсон для максимального потока, Беллман-Форд для минимальной стоимости.

- `FlowNetwork` — наследник `GraphBase<FlowVertex, FlowEdge>`, хранит capacity/cost/flow
- `MaxFlow` — алгоритм Форда-Фалкерсона через BFS в остаточной сети
- `MinCostFlow` — ищет путь минимальной стоимости в остаточке через Беллмана-Форда

Идея: остаточная сеть строится налету. Для каждого ребра `(u,v)` с потоком `f` остаточная пропускная способность = `c - f`. Обратные ребра пока не реализованы явно

### Lab 5: Циклы и TSP

- `EulerianCycle` — проверка (все степени четные) + алгоритм Hierholzer
- `HamiltonianCycle` — бэктрекинг, O(n!) в худшем случае
- `TSPSolver` — полный перебор гамильтоновых циклов, сортировка по весу

TSP работает только на малых графах (n < 10), иначе перебор убивает все.

### Lab 6: Словарь

Хеш-таблица с цепочками vs красно-черное дерево. Интерактивное меню, сохранение в файл, генератор текста.

## Визуализация

### Ядро (core/)

Общие компоненты для всех скриптов визуализации.

- **config.py**: Конфигурация через dataclasses — цвета, размеры, колормапы, шрифты. Разделено на классы (NodeConfig, EdgeConfig и т.д.) для удобства.
- **graph_loader.py**: Загрузка данных из txt-файлов C++. Функции для графов (с весами/потоками), цветов вершин, добавленных рёбер, путей.
- **helpers.py**: Вспомогательные функции — нормализация рёбер, чтение путей, получение рёбер пути.
- **renderer.py**: Класс Renderer с методами setup_plot, compute_layout, draw_nodes, draw_edges, draw_labels, add_legend, finalize. Обёртка над matplotlib/networkx для консистентности.

### Скрипты визуализации

#### plot_graph.py

Рисует граф через `networkx.draw()`. Читает из txt: вершины и рёбра с весами.

#### plot_graph_paths.py

Граф с выделенными путями (для маршрутов и циклов).

#### plot_matrix.py

Матрицы через `seaborn.heatmap()` (Шимбелл, пропускные способности).

#### plot_flow.py

Сети потоков: толщина рёбер = поток, цвет = загрузка.

#### plot_colored_graph.py

Раскраска графов: вершины разных цветов для множеств/покрытий (рёбра не выделяются).

#### plot_flow_animation.py

Анимация роста потока: кадры с увеличивающимся потоком, сохранение в GIF.

#### plot_network.py

Графы с выделенными добавленными рёбрами (для независимых множеств рёбер, покрытий).

Идея: Python скрипты минималистичные, без лишней логики. Вся работа делается в C++, Python только рисует.

## Сборка

```bash
make              # собрать все
make run          # единое меню
make run-lab1     # только lab1
make run-lab3     # только lab3
make run-lab5     # только lab5
make run-lab6     # только lab6
```

Makefile вызывает CMake внутри. Для визуализации нужен активированный venv:

```bash
python3 -m venv venv
source venv/bin/activate
pip install networkx matplotlib seaborn
```

## Единое меню

`main.cc` объединяет все лабы. Использует `std::unique_ptr` для графов, `checkAndRun` для проверки инициализации.

```cpp
template<typename T>
bool checkAndRun(const std::unique_ptr<T>& ptr,
    std::function<void()> action, const char* errorMsg)
{
    if (ptr) {
        action();
        return true;
    }
    std::cout << "[FAIL] " << errorMsg << "\n";
    return false;
}
```

Меню разбито по секциям: общее (генерация + визуализация), lab1, lab3, lab5. Каждая секция запускает свой `Runner`.

## Стиль кода

- C++20: шаблоны, лямбды, `std::optional`, `[[nodiscard]]`
- Namespace `graph` для общего кода, `lab1`/`lab3`/`lab5` для лаб
- Entity-классы вместо процедурного кода (Generator, Visualizer, не статик методы)
