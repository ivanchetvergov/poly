// Figure.h
#pragma once

#include <QPointF>
#include <QPainter>
#include <QColor>

// Figure - class Value Type
class Figure {
    Q_GADGET // отсутствует поддержка сигналов и слотов, и не требует наследования
public:  
    // API для наследников
    virtual ~Figure() = default;

    virtual void draw(QPainter* painter) = 0;
    virtual bool contains(const QPointF& point) const = 0;

    virtual void move(const QPointF& delta){
        pos_ += delta;
    }

    QPointF pos_;      
    QColor color_;  
    bool isActive_ = false; 
};