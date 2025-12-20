// Rect.h
#pragma once

#include "Figure.h"

class Rect : public Figure {
public:
    Rect(const QPointF& position, 
         const QColor& rectColor, 
         qreal width, qreal height
    );

    void draw(QPainter* painter) override;
    bool contains(const QPointF& point) const override;

private:
    // qreal - double|float
    qreal width_;
    qreal height_;
};