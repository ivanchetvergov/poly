// Triag.cc
#include "Triag.h"

Triag::Triag(const QColor& triagColor, const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    color_ = triagColor;
    pos_ = QPointF(qMin(p1.x(), qMin(p2.x(), p3.x())),
                   qMin(p1.y(), qMin(p2.y(), p3.y())));
        
    polygon_.append(p1);
    polygon_.append(p2);
    polygon_.append(p3);
}

void Triag::draw(QPainter* painter) {
    QPen pen(Qt::black, 2);
    painter->setPen(pen);
    painter->setBrush(color_);
    
    painter->drawPolygon(polygon_);
    
    if (isActive_) {
        QPen activePen(Qt::blue, 2, Qt::DashLine);
        painter->setPen(activePen);
        painter->setBrush(Qt::NoBrush);
        
        painter->drawRect(polygon_.boundingRect());
    }
}

bool Triag::contains(const QPointF& point) const {
    return polygon_.containsPoint(point, Qt::OddEvenFill);
    // OddEvenFill это правило заливки для проверки точки внутри н-угольника
}

void Triag::move(const QPointF& delta){
    Figure::move(delta);                            // обновляем центр поз фигуры
    for (int i = 0; i < polygon_.size(); ++i) {     // перебриаем все точки
        polygon_[i] += delta;                       // двигаем точки по отдельности
    }
}