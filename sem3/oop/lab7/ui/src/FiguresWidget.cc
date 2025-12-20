// FigureWidget.cc
#include "FiguresWidget.h"
#include <QPainter>

FiguresWidget::FiguresWidget(QWidget* parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);    // уровень фокусирования (высокий)
    setMouseTracking(true);             // отслеживание мыши
}

FiguresWidget::~FiguresWidget() {
    clearFigures();
}

void FiguresWidget::addFigure(Figure* figure) {
    figures_.append(figure);
    /*
    * update - асинхронный
    кидает запрос на перерисовку в очередь( EventLoop)
    */
    update(); 
}

void FiguresWidget::removeFigure() {
    //* если есть фигуры
    if (!figures_.isEmpty()) {
        //* последняя добавленная или же активная фигруа
        Figure* lastFigure = figures_.last(); 
        // если последний элемент активен, сбрасываем activeFigure_
        if (lastFigure == activeFigure_) {
            activeFigure_ = nullptr;
        }
        figures_.removeLast();      // удаляем последний элемент из вектора
        delete lastFigure;          // освобождаем память
        update();
    }
}

void FiguresWidget::clearFigures() {
    qDeleteAll(figures_);
    figures_.clear();
    activeFigure_ = nullptr;
    update();
}

//--- обработка событий ---

void FiguresWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    //* устанавливаем сглаживание контура
    painter.setRenderHint(QPainter::Antialiasing);

    //* рисуем все фигуры в порядке их добавления ->
    for (Figure* figure : figures_) {
        figure->draw(&painter);
    }
}

void FiguresWidget::mousePressEvent(QMouseEvent* event) {
    //* деактивируем предыдущую фигуру, если она есть
    if (activeFigure_) {
        activeFigure_->isActive_ = false;
        activeFigure_ = nullptr;
    }

    //* ищем фигуру в обратном порядке для выбора верхнего элемента <-
    for (int i = figures_.size() - 1; i >= 0; --i) {
        //* если коорд мыши внутри фигуры
        if (figures_[i]->contains(event->pos())) {
            activeFigure_ = figures_[i];
            activeFigure_->isActive_ = true;
            
            // удаляем фигуру из текущей позиции
            figures_.removeAt(i);
            
            // добавляем её в конец вектора, чтобы она рисовалась поверх остальных
            figures_.append(activeFigure_);
            
            break; // останавливаем поиск
        }
    }
    // обновляем позицию мыши
    lastMousePos_ = event->pos();
    update(); // отрисовка идет -> по коллекции
}

void FiguresWidget::mouseMoveEvent(QMouseEvent* event) {
    //* если есть активная фигура
    if (activeFigure_) {
        QPointF delta = event->pos() - lastMousePos_;    // высчитываем дельту
        activeFigure_->move(delta);                      // двигаем фигуры на нее
        lastMousePos_ = event->pos();                    // обновляем поз мыши
        update();
    }
}

void FiguresWidget::mouseReleaseEvent(QMouseEvent* event) {
    //* если есть активная фигура
    if (activeFigure_) {
        activeFigure_->isActive_ = false;   // деактивация фигуры
        activeFigure_ = nullptr;            // удаление указателя на нее
        update();                           // отрисовка
    }
}