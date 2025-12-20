// Ellipse.h
#pragma once

#include "Figure.h"

#include <QPointF> 
#include <QColor>  
#include <QPainter> 
#include <QRectF>

class Ellipse : public Figure {
public:
    Ellipse(const QPointF& position, 
            const QColor& ellipseColor, 
            qreal width, qreal height);
    
    void draw(QPainter* painter) override;
    bool contains(const QPointF& point) const override;

private:
    qreal width_;
    qreal height_;
};
