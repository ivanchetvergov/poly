// Rect.h
#pragma once

#include "MyVector.h"
#include "Shape.h"

class Rect : public Shape {
public:
    Rect();
    ~Rect() override;
    Rect(const Rect& other);
    Rect(double left, double top, double right, double bottom);
    Rect(const Vector& lt, const Vector& rb);

    Rect& operator=(const Rect& other);
    
    Vector getCenter() const;
    
    void Inflate(double value = 1.0);
    void Inflate(double x_value, double y_value);
    void Inflate(double left, double top, double right, double bottom);

    void Move(const Vector& v) override;
    void Out() const override;
    double Area() const override;
    
private:
    Vector ptLT_; 
    Vector ptRB_;
};