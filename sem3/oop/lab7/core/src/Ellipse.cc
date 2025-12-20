// Ellipse.cc 

#include "Ellipse.h"

Ellipse::Ellipse(const QPointF& position, 
        const QColor& ellipseColor, 
        qreal width, qreal height)
    : width_(width), height_(height) {
    pos_ = position;
    color_ = ellipseColor;
}

void Ellipse::draw(QPainter* painter) {
    QPen pen(Qt::black, 2);
    painter->setPen(pen);
    painter->setBrush(color_);
    
    painter->drawEllipse(pos_.x(), pos_.y(), width_, height_);

    if (isActive_) {
        QPen activePen(Qt::blue, 2, Qt::DashLine);
        painter->setPen(activePen);
        painter->setBrush(Qt::NoBrush);
        
        painter->drawRect(pos_.x(), pos_.y(), width_, height_);
    }
}

bool Ellipse::contains(const QPointF& point) const {
    QRectF rect(pos_.x(), pos_.y(), width_, height_);
    return rect.contains(point);
}