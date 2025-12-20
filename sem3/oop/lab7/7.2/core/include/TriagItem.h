// TriagItem.h
#pragma once

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

// Треугольник на основе QGraphicsPolygonItem
class TriagItem : public QGraphicsPolygonItem {
public:
    TriagItem(const QColor& color, 
              const QPointF& p1, 
              const QPointF& p2, 
              const QPointF& p3,
              QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    // Точные границы треугольника
    QPainterPath shape() const override;

private:
    QColor color_;
    bool isDragging_ = false;
    QPointF lastMousePos_;
    
    void updateAppearance(bool isActive);
};
