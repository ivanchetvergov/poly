#include "presentation/main_window.h"
#include "domain/shape_factory.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QToolBar>
#include <QStatusBar>
#include <QHeaderView>
#include <QRandomGenerator>
#include <QSignalBlocker>

namespace presentation {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // инициализация базы данных
    m_dbManager = std::make_unique<infrastructure::DatabaseManager>();
    if (!m_dbManager->initialize()) {
        QMessageBox::critical(this, "Ошибка", 
            "Не удалось инициализировать базу данных: " + m_dbManager->lastError());
        return;
    }
    
    m_repository = std::make_shared<infrastructure::ShapeRepository>(*m_dbManager);
    
    setupUi();
    setupConnections();
    
    statusBar()->showMessage("Готов к работе");
}

void MainWindow::setupUi() {
    setWindowTitle("Графические примитивы - Qt Application");
    resize(1400, 800);
    
    // центральный виджет со сплиттером
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(centralWidget);
    
    auto* splitter = new QSplitter(Qt::Vertical, this);
    
    // графическое представление
    m_graphicsView = new GraphicsView(m_repository, this);
    splitter->addWidget(m_graphicsView);
    
    // таблица с данными
    m_tableView = new QTableView(this);
    m_shapeModel = new ShapeModel(m_repository, this);
    m_shapeDelegate = new ShapeDelegate(this);
    
    m_tableView->setModel(m_shapeModel);
    m_tableView->setItemDelegateForColumn(ShapeModel::CountColumn, m_shapeDelegate);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    splitter->addWidget(m_tableView);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
    setCentralWidget(centralWidget);
    
    createToolbar();
}

void MainWindow::createToolbar() {
    auto* toolbar = addToolBar("Tools");
    toolbar->setMovable(false);
    
    // * Группа добавления фигур
    auto* addGroup = new QGroupBox("Add figure", this);
    auto* addLayout = new QHBoxLayout(addGroup);
    
    addLayout->addWidget(new QLabel("Type:", this));
    m_shapeTypeCombo = new QComboBox(this);
    m_shapeTypeCombo->addItem("Ellipse", static_cast<int>(domain::ShapeType::Ellipse));
    m_shapeTypeCombo->addItem("Rect", static_cast<int>(domain::ShapeType::Rectangle));
    m_shapeTypeCombo->addItem("Polygon", static_cast<int>(domain::ShapeType::Polygon));
    addLayout->addWidget(m_shapeTypeCombo);
    
    addLayout->addWidget(new QLabel("W:", this));
    m_widthSpin = new QSpinBox(this);
    m_widthSpin->setRange(10, 200);
    m_widthSpin->setValue(80);
    addLayout->addWidget(m_widthSpin);
    
    addLayout->addWidget(new QLabel("H:", this));
    m_heightSpin = new QSpinBox(this);
    m_heightSpin->setRange(10, 200);
    m_heightSpin->setValue(80);
    addLayout->addWidget(m_heightSpin);
    
    auto* addBtn = new QPushButton("Add", this);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddShapeClicked);
    addLayout->addWidget(addBtn);
    
    toolbar->addWidget(addGroup);
    toolbar->addSeparator();
    
    // * Группа связей между фигурами
    auto* connectionGroup = new QGroupBox("Links", this);
    auto* connLayout = new QHBoxLayout(connectionGroup);
    
    connLayout->addWidget(new QLabel("ID:", this));
    m_fromShapeIdSpin = new QSpinBox(this);
    m_fromShapeIdSpin->setMinimum(1);
    connLayout->addWidget(m_fromShapeIdSpin);
    
    connLayout->addWidget(new QLabel("ID:", this));
    m_toShapeIdSpin = new QSpinBox(this);
    m_toShapeIdSpin->setMinimum(1);
    connLayout->addWidget(m_toShapeIdSpin);
    
    auto* addConnBtn = new QPushButton("Link", this);
    connect(addConnBtn, &QPushButton::clicked, this, &MainWindow::onAddConnectionClicked);
    connLayout->addWidget(addConnBtn);
    
    toolbar->addWidget(connectionGroup);
    toolbar->addSeparator();
    
    // * Группа фильтров (по типу и по ID)
    auto* filterGroup = new QGroupBox("Filters", this);
    auto* filterLayout = new QHBoxLayout(filterGroup);
    
    // чекбоксы для фильтрации по типу фигур
    m_filterEllipseCheck = new QCheckBox("Ellipses", this);
    m_filterEllipseCheck->setChecked(true);
    filterLayout->addWidget(m_filterEllipseCheck);
    
    m_filterRectangleCheck = new QCheckBox("Rects", this);
    m_filterRectangleCheck->setChecked(true);
    filterLayout->addWidget(m_filterRectangleCheck);
    
    m_filterPolygonCheck = new QCheckBox("Polygons", this);
    m_filterPolygonCheck->setChecked(true);
    filterLayout->addWidget(m_filterPolygonCheck);

    // чекбокс для включения фильтрации по ID
    m_filterIDActiveCheck = new QCheckBox("Hide by ID", this);
    m_filterIDActiveCheck->setChecked(false); 
    filterLayout->addWidget(m_filterIDActiveCheck);

    // спинбокс для выбора ID фигуры, которую нужно скрыть
    m_filterIDSpin = new QSpinBox(this);
    m_filterIDSpin->setRange(1, 9999);
    m_filterIDSpin->setValue(1);
    m_filterIDSpin->setFixedWidth(60);
    filterLayout->addWidget(m_filterIDSpin);
    
    toolbar->addWidget(filterGroup);
    toolbar->addSeparator();
    
    // * Кнопка удаления выбранной фигуры
    auto* removeBtn = new QPushButton("Remove Selected", this);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveShapeClicked);
    toolbar->addWidget(removeBtn);

}

void MainWindow::setupConnections() {
    // * Подключаем чекбоксы фильтров по типу фигур
    connect(m_filterEllipseCheck, &QCheckBox::checkStateChanged, 
            this, &MainWindow::onFilterChanged);
    connect(m_filterRectangleCheck, &QCheckBox::checkStateChanged, 
            this, &MainWindow::onFilterChanged);
    connect(m_filterPolygonCheck, &QCheckBox::checkStateChanged, 
            this, &MainWindow::onFilterChanged);
    
    // * Подключаем фильтр по ID
    connect(m_filterIDActiveCheck, &QCheckBox::checkStateChanged, 
            this, &MainWindow::onFilterChanged);
    connect(m_filterIDSpin, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::onFilterChanged);
    
    // * Когда модель меняет видимость фигур (через фильтры), обновляем графическое представление
    connect(m_shapeModel, &ShapeModel::shapeVisibilityChanged,
            this, &MainWindow::updateGraphicsView);
    
    // * Когда фигура перемещается, показываем сообщение в статус-баре
    connect(m_graphicsView, &GraphicsView::shapePositionChanged,
            [this](int shapeId, const QPointF& newPos) {
                statusBar()->showMessage(
                    QString("Shape %1 moved into (%2, %3)")
                        .arg(shapeId)
                        .arg(qRound(newPos.x()))
                        .arg(qRound(newPos.y())),
                    3000);
            });
}

void MainWindow::onAddShapeClicked() {
    // получаем выбранный тип фигуры из комбобокса
    auto type = static_cast<domain::ShapeType>(m_shapeTypeCombo->currentData().toInt());
    
    // генерируем случайные координаты для новой фигуры
    auto* generator = QRandomGenerator::global();
    qreal randX = generator->bounded(-450, 551);
    qreal randY = generator->bounded(-250, 151);

    // создаём данные фигуры с дефолтными параметрами и случайной позицией
    auto shapeData = domain::ShapeFactory::createDefaultShapeData(
        type, 
        QPointF(randX, randY)
    );
    
    // устанавливаем размеры из UI
    shapeData.width = m_widthSpin->value();
    shapeData.height = m_heightSpin->value();
    
    // сохраняем в БД
    auto id = m_repository->save(shapeData);
    if (id) {
        statusBar()->showMessage(QString("Фигура добавлена с ID: %1").arg(*id), 3000);
        m_shapeModel->refresh();      // обновляем таблицу
        updateGraphicsView();         // обновляем сцену (repository уже эмитил shapeAdded, но на всякий случай)
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить фигуру");
    }
}

void MainWindow::onRemoveShapeClicked() {
    // * Получаем выбранную строку из таблицы
    auto selectedIndexes = m_tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::information(this, "Информация", "Выберите фигуру для удаления");
        return;
    }
    
    int row = selectedIndexes.first().row();
    auto shapeData = m_shapeModel->getShapeData(row);
    
    // * Просим подтверждение удаления
    auto reply = QMessageBox::question(this, "Подтверждение",
        QString("Удалить фигуру с ID %1?").arg(shapeData.id),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // сбрасываем выделение перед удалением
        m_tableView->selectionModel()->clearSelection();
        m_tableView->setCurrentIndex(QModelIndex());

        if (m_shapeModel->removeRow(row)) {
            statusBar()->showMessage("Фигура удалена", 3000);
            updateGraphicsView();  // обновляем сцену
            m_tableView->selectionModel()->clearSelection();

            // * Автоматически выбираем следующую строку после удаления
            if (m_shapeModel->rowCount() > 0) {
                // выбираем строку на месте удалённой или последнюю, если удалили последнюю
                int newRow = std::min(row, m_shapeModel->rowCount() - 1);
                
                QModelIndex newIndex = m_shapeModel->index(newRow, 0);
                m_tableView->setCurrentIndex(newIndex);
                m_tableView->selectionModel()->select(
                    newIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows
                );
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить фигуру");
        }
    }
}

void MainWindow::onAddConnectionClicked() {
    // * Получаем ID фигур для создания связи
    int fromId = m_fromShapeIdSpin->value();
    int toId = m_toShapeIdSpin->value();
    
    // * Проверяем, что пользователь не пытается связать фигуру саму с собой
    if (fromId == toId) {
        QMessageBox::warning(this, "Ошибка", "Нельзя связать фигуру саму с собой");
        return;
    }
    
    // * Проверяем, что обе фигуры существуют в БД
    auto fromShape = m_repository->findById(fromId);
    auto toShape = m_repository->findById(toId);
    
    if (!fromShape || !toShape) {
        QMessageBox::warning(this, "Ошибка", "Одна или обе фигуры не найдены");
        return;
    }
    
    // * Создаём связь в БД
    if (m_repository->addConnection(fromId, toId)) {
        statusBar()->showMessage(
            QString("Связь создана: %1 -> %2").arg(fromId).arg(toId), 3000);
        m_shapeModel->refresh();   // обновляем таблицу (там отображается количество связей)
        updateGraphicsView();      // обновляем сцену (чтобы нарисовать линию связи)
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать связь");
    }
}

void MainWindow::onFilterChanged() {
    // Атомарно обновляем все три фильтра по типу фигур в модели.
    // Это предотвращает множественные refresh сцены - вместо этого модель
    // эмитит один сигнал shapeVisibilityChanged после обновления всех фильтров.
    m_shapeModel->setTypeFilters(
        m_filterEllipseCheck->isChecked(),
        m_filterRectangleCheck->isChecked(),
        m_filterPolygonCheck->isChecked()
    );

    // получаем состояние фильтра по ID
    bool isIdFilterActive = m_filterIDActiveCheck->isChecked();
    int targetId = m_filterIDSpin->value();
    
    // Обновляем видимость каждой фигуры на основе комбинации всех фильтров.
    // Это работает с уже загруженными элементами сцены и не пересоздаёт их заново.
    auto allShapes = m_repository->findAll();
    for (const auto& shapeData : allShapes) {
        bool shouldBeVisible = true;
        
        // проверяем фильтры по типу
        if (shapeData.type == domain::ShapeType::Ellipse && !m_filterEllipseCheck->isChecked()) {
            shouldBeVisible = false;
        } else if (shapeData.type == domain::ShapeType::Rectangle && !m_filterRectangleCheck->isChecked()) {
            shouldBeVisible = false;
        } else if (shapeData.type == domain::ShapeType::Polygon && !m_filterPolygonCheck->isChecked()) {
            shouldBeVisible = false;
        }

        // проверяем фильтр по ID (если он активен и ID совпадает - скрываем)
        if (isIdFilterActive && shapeData.id == targetId) {
            shouldBeVisible = false;
        }
        
        // применяем изменение видимости к фигуре на сцене
        m_graphicsView->updateShapeVisibility(shapeData.id, shouldBeVisible);
    }
}

void MainWindow::onRefreshClicked() {
    // * Полное обновление всех данных из БД (таблица + сцена)
    m_shapeModel->refresh();
    updateGraphicsView();
    statusBar()->showMessage("Данные обновлены", 2000);
}

void MainWindow::updateGraphicsView() {
    // * Обновляем графическое представление (полная перезагрузка сцены из БД)
    m_graphicsView->refresh();
}

} // namespace presentation