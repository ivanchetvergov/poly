// MainWindow.cc
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <ctime>

#include "Rect.h"
#include "Triag.h"
#include "Ellipse.h"

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent) {
    //* инициалазиация генератора псевдослучайных чисел
    std::srand(std::time(nullptr)); 
    setupUi(); // вызов мтеода для настройки
}

void MainWindow::setupUi() {
    // 1 -- настройка главного окна
    setWindowTitle("Qt Figures");
    setMinimumSize(800, 600);

    // 2 --- создание основного виджета для отрисовки
    figuresWidget_ = new FiguresWidget(this); // this как родитель

    // 3 --- создание эл-ов управления
    figureTypeComboBox_ = new QComboBox(this); // выпадающий список
    // элементы этого спика
    figureTypeComboBox_->addItem("Rectangle");
    figureTypeComboBox_->addItem("Triangle");
    figureTypeComboBox_->addItem("Ellipse");
    
    // добавляем кнопки (add/remove)
    addButton_ = new QPushButton("Add Figure", this);
    removeButton_ = new QPushButton("Remove Figure", this);

    // 4 --- компоновка элементов управления
    QHBoxLayout* controlLayout = new QHBoxLayout;   // гориз. макет <->
    // добавляем эл-ты в макет
    controlLayout->addWidget(figureTypeComboBox_);  // список фигур
    controlLayout->addWidget(addButton_);           // add кнопка
    controlLayout->addWidget(removeButton_);        // rm кнопка 
    controlLayout->addStretch();                    // растягивание по ширине

    // 5 --- финальная компоновка (верт макет)
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(controlLayout);           // блок управления
    mainLayout->addWidget(figuresWidget_);          // блок фигур (займет все ост. про-во)

    // 6 --- установка центр виджета
    //* QMainWindow требует центрального виджета для размещения пользовательского контента
    QWidget* centralWidget = new QWidget(this);     // создаем центр виджет
    centralWidget->setLayout(mainLayout);           // назначаем ему конечную компоновку
    setCentralWidget(centralWidget);                // устанвливаем виджет как центральный 

    // 7 --- соединяем сигналы и слоты
    connect(addButton_, &QPushButton::clicked, this, &MainWindow::on_add_button_clicked);
    connect(removeButton_, &QPushButton::clicked, this, &MainWindow::on_remove_button_clicked);
}

//* slot для кнопки add 
void MainWindow::on_add_button_clicked() {
    // 1 --- определение типа фигуры
    QString selectedType = figureTypeComboBox_->currentText();
    Figure* newFigure = nullptr;
    
    // 2 --- генерация случайных параметров 
    // для позиции делаем отступ в 50 от краев виджета
    QPointF pos(std::rand() % (figuresWidget_->width() - 50), std::rand() % (figuresWidget_->height() - 50));
    QColor color(std::rand() % 256, std::rand() % 256, std::rand() % 256); // фулл рандом

    // 3 --- создание новой фигуры
    if (selectedType == "Rectangle") {
        newFigure = new Rect(pos, color, 100, 70);
    } else if (selectedType == "Triangle") {
        // создаем треуг фиксированной формы
        QPointF p1 = pos;
        QPointF p2(pos.x() + 100, pos.y());
        QPointF p3(pos.x() + 50, pos.y() - 100);
        newFigure = new Triag(color, p1, p2, p3);
    } else if (selectedType == "Ellipse") {
        newFigure = new Ellipse(pos, color, 100, 70);
    }

    // 4 --- добавляем фигуру в виджет
    if (newFigure) {
        // за логику добавления отвечает отдельный виджет
        figuresWidget_->addFigure(newFigure);
    }
}

//* slot для кнопки rm
void MainWindow::on_remove_button_clicked() {
    // передаем отдельному виджету
    figuresWidget_->removeFigure();
}