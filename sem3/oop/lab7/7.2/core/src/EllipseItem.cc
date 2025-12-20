// EllipseItem.cc
#include "EllipseItem.h"
#include "FiguresScene.h"

EllipseItem::EllipseItem(const QPointF& position, 
                         const QColor& color, 
                         qreal width, qreal height,
                         QGraphicsItem* parent)
    : QGraphicsEllipseItem(0, 0, width, height, parent), color_(color) 
{
    // Устанавливаем позицию
    setPos(position);
    
    // Включаем флаги для взаимодействия
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    
    // Устанавливаем внешний вид
    updateAppearance(false);
}

void EllipseItem::updateAppearance(bool isActive) {
    setBrush(QBrush(color_));
    
    if (isActive) {
        setPen(QPen(Qt::blue, 2, Qt::DashLine));
    } else {
        setPen(QPen(Qt::black, 2));
    }
}

QPainterPath EllipseItem::shape() const {
    // Точные границы эллипса (не прямоугольник!)
    QPainterPath path;
    path.addEllipse(rect());
    return path;
}

void EllipseItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging_ = true;
        lastMousePos_ = event->scenePos();
        
        // Поднимаем фигуру на передний план через сцену
        if (FiguresScene* figScene = qobject_cast<FiguresScene*>(scene())) {
            figScene->bringToFront(this);
        }
        
        updateAppearance(true); // обновляем вид
        update();
    }
    // вызываем стандартную реадализациюй
    QGraphicsEllipseItem::mousePressEvent(event);
}

void EllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
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

void EllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging_ = false;
        updateAppearance(false);
        update();
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}
