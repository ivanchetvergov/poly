// Rect.cc
#include "Rect.h"

Rect::Rect(const QPointF& position, 
            const QColor& rectColor, 
            qreal width, qreal height)
    : width_(width), height_(height) { 
    this->pos_ = position;
    this->color_ = rectColor;
}


void Rect::draw(QPainter* painter) {
    QPen pen(Qt::black, 2);
    painter->setPen(pen);
    painter->setBrush(color_);

    painter->drawRect(pos_.x(), pos_.y(), width_, height_);

    if (isActive_) {
        QPen activePen(Qt::blue, 2, Qt::DashLine);
        painter->setPen(activePen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(pos_.x(), pos_.y(), width_, height_);
    }
}

bool Rect::contains(const QPointF& point) const {
    QRectF rect(pos_.x(), pos_.y(), width_, height_);
    return rect.contains(point); // contains метод QRectF
}