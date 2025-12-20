// EllipseItem.h
#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>

// Эллипс на основе QGraphicsEllipseItem
class EllipseItem : public QGraphicsEllipseItem {
public:
    EllipseItem(const QPointF& position, 
                const QColor& color, 
                qreal width, qreal height,
                QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    // Точные границы эллипса
    QPainterPath shape() const override;

private:
    QColor color_;
    bool isDragging_ = false;
    QPointF lastMousePos_;
    
    void updateAppearance(bool isActive);
};
