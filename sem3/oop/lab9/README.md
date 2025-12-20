# Приложение для работы с графическими примитивами

## Описание

Приложение на Qt для создания, управления и визуализации графических примитивов (эллипсов, прямоугольников, многоугольников) с поддержкой связей между ними. Все данные хранятся в SQLite базе данных.

## Архитектура

Проект следует принципам **чистой архитектуры** и разделен на три основных слоя:

```bash
src/
├── domain/              # Бизнес-логика и модели
│   ├── shape.cpp/h
│   ├── shape_types.h
│   └── shape_factory.cpp/h
├── infrastructure/      # Работа с базой данных
│   ├── database_manager.cpp/h
│   └── shape_repository.cpp/h
└── presentation/        # UI слой
    ├── main_window.cpp/h
    ├── graphics_view.cpp/h
    ├── shape_model.cpp/h
    └── shape_delegate.cpp/h
```

### Ключевые особенности:

- **C++17/20**: Использование modern C++ фичей (`std::optional`, `[[nodiscard]]`, structured bindings)
- **Boost.Signals2**: Для реализации паттерна наблюдатель между слоями
- **RAII**: Правильное управление ресурсами
- **Smart Pointers**: `std::unique_ptr`, `std::shared_ptr`
- **Const correctness**: `noexcept` спецификаторы
- **SQLite**: Через Qt SQL модуль

## Требования

- **CMake** >= 3.16
- **Qt6** (Core, Gui, Widgets, Sql)
- **Boost** >= 1.70 (только headers, Signals2)
- **C++20** совместимый компилятор (GCC 10+, Clang 10+, MSVC 2019+)

## Сборка

### Linux/macOS

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./GraphicsPrimitives
```

### Windows (Visual Studio)

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
Release\GraphicsPrimitives.exe
```

### С помощью Qt Creator

1. Откройте `CMakeLists.txt` как проект
2. Настройте kit (Qt6 + компилятор C++20)
3. Нажмите Build → Run

## Функциональность

### Основные возможности:

1. **Добавление фигур**:
   - Эллипсы, прямоугольники, многоугольники
   - Настройка позиции, размеров
   - Автоматическое сохранение в БД

2. **Управление связями**:
   - Создание направленных связей между фигурами
   - Визуализация связей линиями со стрелками
   - Каскадное удаление при удалении фигуры

3. **Фильтрация**:
   - Скрытие фигур по типу без удаления из БД
   - Мгновенное обновление представления

4. **Визуализация**:
   - Графическое отображение всех фигур
   - Сетка для удобства позиционирования
   - Связи с направленными стрелками

5. **Табличное представление**:
   - Кастомный делегат для отображения иконок
   - 1-3 фигуры → 1 иконка
   - 4-10 фигур → 2 иконки
   - >10 фигур → 3 иконки

### Структура БД:

#### Таблица `shapes`:
```sql
- id (INTEGER PRIMARY KEY)
- type (TEXT)
- position_x, position_y (REAL)
- width, height (REAL)
- polygon_points (TEXT/JSON)
- is_visible (INTEGER)
```

#### Таблица `shape_connections`:
```sql
- id (INTEGER PRIMARY KEY)
- shape_id (INTEGER, FK)
- connected_shape_id (INTEGER, FK)
```

## Примеры использования

### Добавление фигуры:

1. Выберите тип фигуры в выпадающем списке
2. Задайте координаты X, Y
3. Укажите размеры (ширина, высота)
4. Нажмите "Добавить"

### Создание связи:

1. Введите ID исходной фигуры ("От ID")
2. Введите ID целевой фигуры ("К ID")
3. Нажмите "Связать"

### Фильтрация:

- Снимите галочку с типа фигур, которые хотите скрыть
- Фигуры исчезнут из представления, но останутся в БД

### Удаление:

1. Выберите строку в таблице
2. Нажмите "Удалить выбранное"
3. Подтвердите удаление
4. Все связи с этой фигурой также удалятся

## Boost + Qt

В проекте используется **Boost.Signals2** для реализации event-driven архитектуры:

```cpp
// В ShapeRepository
boost::signals2::signal<void(int)> shapeAdded;
boost::signals2::signal<void(int)> shapeRemoved;

// Подписка в GraphicsView
m_repository->shapeAdded.connect([this](int id) { 
    onShapeAdded(id); 
});
```

Boost отлично интегрируется с Qt - никаких конфликтов сигналов, так как Boost использует свою систему.

## Стиль кода

Код следует современным стандартам C++:

- **RAII everywhere**
- **Rule of Five** (или Zero)
- **Const correctness**
- **noexcept** где возможно
- **[[nodiscard]]** для важных возвращаемых значений
- **Smart pointers** вместо raw pointers
- **Range-based for loops**
- **Structured bindings** (C++17)
- **std::optional** вместо null checks

## Расширение функциональности

### Добавление нового типа фигуры:

1. Добавьте тип в `ShapeType` enum (`shape_types.h`)
2. Создайте класс, наследующий `Shape` (`shape.h`)
3. Реализуйте `draw()` и `boundingRect()`
4. Добавьте в `ShapeFactory::create()`
5. Обновите UI (combo box, делегат)

### Добавление новых свойств:

1. Расширьте `ShapeData` структуру
2. Обновите SQL схему в `DatabaseManager`
3. Модифицируйте `ShapeRepository` для сохранения/загрузки
4. Добавьте UI элементы в `MainWindow`

## Производительность

- **Lazy loading**: Фигуры загружаются только при необходимости
- **SQL индексы**: Внешние ключи для быстрого поиска связей
- **View updates**: Только измененные области
- **Smart invalidation**: Перерисовка только при изменениях

## Лицензия

Учебный проект. Свободное использование.

## Автор

Создано с использованием современных паттернов Qt и C++20.