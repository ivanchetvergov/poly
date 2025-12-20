// Triag.h
#pragma once

#include "Figure.h"
#include <QPolygonF> 

class Triag : public Figure {
public:
    Triag(const QColor& triagColor, const QPointF& p1, 
          const QPointF& p2, const QPointF& p3);
    
    void draw(QPainter* painter) override;
    bool contains(const QPointF& point) const override;
    void move(const QPointF& delta) override;

private:
    // QPolygonF — QVector<QPointF>
    QPolygonF polygon_;
};