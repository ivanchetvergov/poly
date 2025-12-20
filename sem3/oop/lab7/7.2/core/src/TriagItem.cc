// TriagItem.cc
#include "TriagItem.h"
#include "FiguresScene.h"

TriagItem::TriagItem(const QColor& color, 
                     const QPointF& p1, 
                     const QPointF& p2, 
                     const QPointF& p3,
                     QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent), color_(color) 
{
    // Создаем полигон треугольника
    QPolygonF polygon;
    polygon << p1 << p2 << p3;
    setPolygon(polygon);
    
    // Включаем флаги для взаимодействия
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    
    // Устанавливаем внешний вид
    updateAppearance(false);
}

void TriagItem::updateAppearance(bool isActive) {
    setBrush(QBrush(color_));
    
    if (isActive) {
        setPen(QPen(Qt::blue, 2, Qt::DashLine));
    } else {
        setPen(QPen(Qt::black, 2));
    }
}

QPainterPath TriagItem::shape() const {
    // Точные границы треугольника (полигон)
    QPainterPath path;
    path.addPolygon(polygon());
    path.closeSubpath();
    return path;
}

void TriagItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging_ = true;
        lastMousePos_ = event->scenePos();
        
        // Поднимаем фигуру на передний план через сцену
        if (FiguresScene* figScene = qobject_cast<FiguresScene*>(scene())) {
            figScene->bringToFront(this);
        }
        
        updateAppearance(true);
        update();
    }
    QGraphicsPolygonItem::mousePressEvent(event);
}

void TriagItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging_) {
        QPointF delta = event->scenePos() - lastMousePos_;

        QRectF curSceneRect = mapToScene(boundingRect()).boundingRect();
        QRectF sceneRect = scene() ? scene()->sceneRect() : QRectF();
        QRectF proposed = curSceneRect.translated(delta);

        qreal dx = delta.x();
        qreal dy = delta.y();
        if (!sceneRect.isNull()) {
            if (proposed.left() < sceneRect.left()) dx += sceneRect.left() - proposed.left();
            if (proposed.right() > sceneRect.right()) dx -= proposed.right() - sceneRect.right();
            if (proposed.top() < sceneRect.top()) dy += sceneRect.top() - proposed.top();
            if (proposed.bottom() > sceneRect.bottom()) dy -= proposed.bottom() - sceneRect.bottom();
        }

        moveBy(dx, dy);
        lastMousePos_ = lastMousePos_ + QPointF(dx, dy);
    }
}

void TriagItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging_ = false;
        updateAppearance(false);
        update();
    }
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}
