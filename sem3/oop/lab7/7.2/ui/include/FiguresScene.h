// FiguresScene.h
#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

// Сцена для управления коллекцией фигур
// Использует QGraphicsScene для автоматического управления Z-order и событиями
class FiguresScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit FiguresScene(QObject* parent = nullptr);
    ~FiguresScene() override = default;

    // Методы для работы с фигурами
    void addRectangle(const QPointF& position, const QColor& color, 
                      qreal width, qreal height);
    void addEllipse(const QPointF& position, const QColor& color, 
                    qreal width, qreal height);
    void addTriangle(const QColor& color, const QPointF& p1, 
                     const QPointF& p2, const QPointF& p3);
    
    // Удаление последней добавленной фигуры
    void removeLastFigure();
    
    // Очистка всех фигур
    void clearAllFigures();
    
    // Поднять фигуру на передний план
    void bringToFront(QGraphicsItem* item);
    
    // Получить следующий Z-Value
    qreal getNextZValue() { return currentZValue_++; }

private:
    // Храним указатели на фигуры в порядке добавления
    QList<QGraphicsItem*> figuresOrder_;
    
    // Текущий Z-index для новых фигур
    qreal currentZValue_ = 0;
};
