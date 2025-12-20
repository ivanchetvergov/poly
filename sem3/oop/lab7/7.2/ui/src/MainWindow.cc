// MainWindow.cc
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <ctime>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent) {
    // Инициализация генератора псевдослучайных чисел
    std::srand(std::time(nullptr)); 
    setupUi();
}

void MainWindow::setupUi() {
    // 1 --- настройка главного окна
    setWindowTitle("Qt Figures 7.2");
    setMinimumSize(800, 600);

    // 2 --- создание сцены и представления
    figuresScene_ = new FiguresScene(this);
    graphicsView_ = new QGraphicsView(figuresScene_, this);
    
    // Настройка представления
    graphicsView_->setRenderHint(QPainter::Antialiasing);
    graphicsView_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    graphicsView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 3 --- создание элементов управления
    figureTypeComboBox_ = new QComboBox(this);
    figureTypeComboBox_->addItem("Rectangle");
    figureTypeComboBox_->addItem("Triangle");
    figureTypeComboBox_->addItem("Ellipse");
    
    addButton_ = new QPushButton("Add Figure", this);
    removeButton_ = new QPushButton("Remove Figure", this);

    // 4 --- компоновка элементов управления
    QHBoxLayout* controlLayout = new QHBoxLayout;
    controlLayout->addWidget(figureTypeComboBox_);
    controlLayout->addWidget(addButton_);
    controlLayout->addWidget(removeButton_);
    controlLayout->addStretch();

    // 5 --- финальная компоновка
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(graphicsView_);

    // 6 --- установка центрального виджета
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 7 --- соединяем сигналы и слоты
    connect(addButton_, &QPushButton::clicked, this, &MainWindow::on_add_button_clicked);
    connect(removeButton_, &QPushButton::clicked, this, &MainWindow::on_remove_button_clicked);
}

void MainWindow::on_add_button_clicked() {
    // 1 --- определение типа фигуры
    QString selectedType = figureTypeComboBox_->currentText();
    
    // 2 --- генерация случайных параметров
    QRectF sceneRect = figuresScene_->sceneRect();
    QPointF pos(std::rand() % static_cast<int>(sceneRect.width() - 100),
                std::rand() % static_cast<int>(sceneRect.height() - 100));
    QColor color(std::rand() % 256, std::rand() % 256, std::rand() % 256);

    // 3 --- создание новой фигуры через сцену
    if (selectedType == "Rectangle") {
        figuresScene_->addRectangle(pos, color, 100, 70);
    } else if (selectedType == "Triangle") {
        QPointF p1(0, 0);
        QPointF p2(100, 0);
        QPointF p3(50, -100);
        // Треугольник создается в локальных координатах, позиция через setPos
        figuresScene_->addTriangle(color, 
                                   pos + p1, 
                                   pos + p2, 
                                   pos + p3);
    } else if (selectedType == "Ellipse") {
        figuresScene_->addEllipse(pos, color, 100, 70);
    }
}

void MainWindow::on_remove_button_clicked() {
    figuresScene_->removeLastFigure();
}
