// RectItem.h
#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>

// Прямоугольник на основе QGraphicsRectItem
class RectItem : public QGraphicsRectItem {
public:
    RectItem(const QPointF& position, 
             const QColor& color, 
             qreal width, qreal height,
             QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    // Точные границы для определения попадания курсора
    QPainterPath shape() const override;

private:
    QColor color_;
    bool isDragging_ = false;
    QPointF lastMousePos_;
    
    void updateAppearance(bool isActive);
};
