# Common Module Documentation

Здесь собраны все базовые компоненты для работы с графами, визуализацией и утилитами. Common — это core модуль. Все остальное завязано на нем.

## GraphBase

Базовый шаблонный класс для графов: `GraphBase<VertexT, EdgeT>`. Управляет вершинами и ребрами.

Ключевые методы:

- `vertexIds()` - возвращает вектор ID вершин.
- `edges()` - список всех рёбер.
- `hasEdge(from, to)` - проверка наличия ребра.
- `getEdgeWeight(from, to)` - вес ребра (optional).
- `addEdge(from, to, weight)` - добавление ребра.
- `neighbors(v)` - список соседей вершины.

## Graph

Наследник GraphBase с `Vertex = int`, `EdgeData = double` (вес). Стандартный граф.

Дополнительно:

- `isDirected()` - ориентированный ли граф.

## FlowNetwork

Класс для сетей потоков. `Vertex = FlowVertex`, `Edge = FlowEdge` (capacity, flow, cost).

Методы:

- `getCapacity(from, to)` - пропускная способность.
- `getFlow(from, to)` - текущий поток.
- `getCost(from, to)` - стоимость ребра.
- `addFlow(from, to, amount)` - увеличить поток.

## Generator

Генератор графов. Методы для создания разных типов.

Методы:

- `generateAcyclicGraph(vertices, edges, directed)` - ациклический граф.
- `generateConnectedGraph(vertices, edges, directed)` - связный граф.

## Visualizer

Визуализатор через Python-скрипты. Все методы статические.

Методы:

- `draw(data, directed, type)` - отрисовка графа или сети.
- `drawPaths(data, directed, type)` - с выделенными путями.
- `drawMatrix(data, title)` - матрица.
- `drawHashTable(txt, png, title)` - хэш-таблица (для Lab6).
- `drawRBTree(txt, png, title)` - красно-чёрное дерево.
- `drawTextStats(txt, png, title)` - статистика текста.

## FileHandler

Обработчик файлов для сохранения/загрузки данных. Всё статическое.

Методы:

- `saveGraph(filename, graph)` - сохранить граф.
- `saveAdjacencyMatrix(filename, graph)` - матрица смежности.
- `saveWeightMatrix(filename, graph)` - матрица весов.
- `saveCapacityMatrix(filename, network)` - пропускных способностей.
- `saveCostMatrix(filename, network)` - стоимостей.
- `saveDistanceMatrix(filename, matrix)` - расстояний (матрица optional(double)).
- `savePath(filename, path)` - один путь.
- `savePaths(filename, paths)` - несколько путей.
- `saveColors(filename, vertices, colors)` - раскраска вершин.
- `saveMatrix(filename, matrix)` - произвольная double-матрица.

## CollectionUtils

Утилиты для коллекций. Шаблонные хелперы.

Методы:

- `makeMatrix(rows, cols, getter)` - создать матрицу с лямбдой.
- `makeSquareMatrix(ids, getter)` - квадратную матрицу для графа.

## DrawDataConfig

Конфигурация для визуализации. Хранит пути и заголовки для каждого пункта меню.

Методы:

- `getConfigs()` - мапа ID -> DrawData.

## Menu

Главное меню приложения. Управляет графами, сетями и запуском лаб.

Методы:

- `show()` - отобразить меню.
- `handleChoice(choice)` - обработать выбор пользователя.

Внутри — куча actions с лямбдами для каждого пункта.

## PathUtils

Утилиты для работы с путями. Шаблонные функции.

Методы:

- `forEachEdgeInPath(graph, path, func)` - применить функцию к каждому ребру в пути.

## Utils

Общие утилиты.

Функции:

- `readInt(prompt)` - чтение int из консоли.
- `checkAndRun(ptr, action, msg)` - проверка указателя и выполнение действия.
- `runPythonScript(script, args)` - запуск Python-скрипта.
