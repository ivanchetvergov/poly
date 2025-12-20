// FiguresWidget.h
#pragma once

#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QPointF>

#include "Figure.h"

class FiguresWidget : public QWidget {
    Q_OBJECT

public:
    explicit FiguresWidget(QWidget* parent = nullptr);
    ~FiguresWidget() override;

    void addFigure(Figure* figure);
    void removeLastFigure();
    void clearFigures();

    void removeFigure();
    
protected:
    //* FiguresWidget получатель событий, получая их вызывает override методы
    void paintEvent(QPaintEvent* event) override;         // отрисовка
    void mousePressEvent(QMouseEvent* event) override;    // нажатие кнопки мыши
    void mouseMoveEvent(QMouseEvent* event) override;     // движение мыши с нажатой кнопкой
    void mouseReleaseEvent(QMouseEvent* event) override;  // отжатие) кнопки мышки (release)

private:
    QVector<Figure*> figures_;
    Figure* activeFigure_ = nullptr;
    QPointF lastMousePos_;
};