# Graph Labs

Лабы по теории графов на C++20. Единое меню, визуализация через Python, модульная архитектура.

## Структура

```bush
graph/
├── main.cc                  # Единое меню для всех лаб
├── common/                  # Переиспользуемый код
│   ├── include/
│   │   ├── GraphBase.h      # Template базовый класс для всех графов
│   │   ├── Graph.h          # Обычный граф (взвешенный, ор/неор)
│   │   ├── Generator.h      # Генератор графов (дерево, ацикл, связный)
│   │   ├── Visualizer.h     # Врапер для вызова питоновских скриптов
│   │   ├── PathUtils.h      # Template-утилиты для путей (поиск мин, apply)
│   │   ├── CollectionUtils.h # Хелперы для векторов/мапов
│   │   └── Utils.h          # Меню, чтение инпута, checkAndRun
│   └── src/
│       ├── GraphBase.tpp    # Реализация template-методов
│       ├── Graph.cc         # Реализация Graph
│       ├── Generator.cc     # Генерация графов
│       └── Visualizer.cc    # Экспорт в txt + system() для Python
├── lab1/                    # Метод Шимбелла и подсчет путей
├── lab3/                    # Потоки (Форд-Фалкерсон, минимальная стоимость)
├── lab5/                    # Циклы (Эйлер, Гамильтон, TSP)
├── lab6/                    # Структуры данных (хеш-таблица, RB-дерево)
├── scripts/                 # Python для визуализации
│   ├── plot_graph.py        # Граф через networkx
│   ├── plot_graph_path.py   # Граф с выделенным путем
│   ├── plot_matrix.py       # Матрица через seaborn
│   └── plot_flow.py         # Сеть потоков с capacity/flow
└── assets/
    ├── png/                 # Готовые картинки
    └── txt/                 # Промежуточные данные для Python
```

## Архитектура

### GraphBase<VertexT, EdgeT>

Базовый template-класс для всех графов. Параметризуется типом вершины и ребра.

```cpp
template<typename VertexT, typename EdgeT>
class GraphBase {
    bool isDirected_;
    unordered_map<int, unique_ptr<VertexT>> m_vertices;
    unordered_map<string, EdgeT> m_edges;  // ключ = makeKey(from, to)
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

## Lab 1: Метод Шимбелла и подсчет путей

Ищет кратчайшие/длиннейшие пути заданной длины через матричное умножение. Алгоритм: вместо обычного умножения матриц делаем `A^k[i][j] = min(A^(k-1)[i][t] + A[t][j])`.

- `ShimbellMethod` — template-класс, параметризуется операцией (min/max)
- `PathCounter` — рекурсивный DFS с мемоизацией для подсчета всех маршрутов

## Lab 3: Потоки в сетях

Форд-Фалкерсон для максимального потока, Беллман-Форд для минимальной стоимости.

- `FlowNetwork` — наследник `GraphBase<FlowVertex, FlowEdge>`, хранит capacity/cost/flow
- `MaxFlow` — алгоритм Форда-Фалкерсона через BFS в остаточной сети
- `MinCostFlow` — ищет путь минимальной стоимости в остаточке через Беллмана-Форда

Идея: остаточная сеть строится налету. Для каждого ребра `(u,v)` с потоком `f` остаточная пропускная способность = `c - f`. Обратные ребра пока не реализованы явно (в TODO).

## Lab 5: Циклы и TSP

- `EulerianCycle` — проверка (все степени четные) + алгоритм Hierholzer
- `HamiltonianCycle` — бэктрекинг, O(n!) в худшем случае
- `TSPSolver` — полный перебор гамильтоновых циклов, сортировка по весу

TSP работает только на малых графах (n < 10), иначе перебор убивает все.

## Lab 6: Словарь

Хеш-таблица с цепочками vs красно-черное дерево. Интерактивное меню, сохранение в файл, генератор текста.

## Визуализация

### plot_graph.py

Рисует граф через `networkx.draw()`. Читает из txt:

```txt
vertices: 0 1 2 3
edges: 0 1 5.2
       1 2 3.1
       2 3 4.5
```

Вершины — круги, ребра — линии с весами. Для ориентированных графов добавляет стрелки.

### plot_graph_path.py

То же, но выделяет путь красным цветом. Формат txt включает секцию `path:`.

```txt
vertices: 0 1 2 3
edges: 0 1 5.2
       1 2 3.1
path: 0 1 2
```

### plot_matrix.py

Рисует heatmap через `seaborn`. Читает матрицу NxN, где `inf` или пустое место = белый цвет.

```txt
3
0 5 inf
inf 0 3
2 inf 0
```

### plot_flow.py

Рисует сеть потоков с подписями `capacity/flow`. Насыщенные ребра (flow = capacity) выделяет жирной линией.

```txt
vertices: 0 1 2 3
edges: 0 1 10 5    # from to capacity flow
       1 2 8 8
       2 3 15 8
```

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
