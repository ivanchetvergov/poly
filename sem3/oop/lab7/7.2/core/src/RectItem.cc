// RectItem.cc
#include "RectItem.h"
#include "FiguresScene.h"

RectItem::RectItem(const QPointF& position, 
                   const QColor& color, 
                   qreal width, qreal height,
                   QGraphicsItem* parent)
    : QGraphicsRectItem(0, 0, width, height, parent), color_(color) 
{
    // Устанавливаем позицию
    setPos(position);
    
    // Включаем флаги для взаимодействия
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    
    // Устанавливаем внешний вид
    updateAppearance(false);
}

void RectItem::updateAppearance(bool isActive) {
    setBrush(QBrush(color_));
    
    if (isActive) {
        setPen(QPen(Qt::blue, 2, Qt::DashLine));
    } else {
        setPen(QPen(Qt::black, 2));
    }
}

QPainterPath RectItem::shape() const {
    // Точные границы прямоугольника
    QPainterPath path;
    path.addRect(rect());
    return path;
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
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
    QGraphicsRectItem::mousePressEvent(event);
}

void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging_) {
        QPointF delta = event->scenePos() - lastMousePos_;

        // Текущий прямоугольник в координатах сцены
        QRectF curSceneRect = mapToScene(boundingRect()).boundingRect();
        QRectF sceneRect = scene() ? scene()->sceneRect() : QRectF();

        // Предлагаемый прямоугольник после перемещения
        QRectF proposed = curSceneRect.translated(delta);

        // Подкорректируем дельту, чтобы не выйти за пределы сцены
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

void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging_ = false;
        updateAppearance(false);
        update();
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}
