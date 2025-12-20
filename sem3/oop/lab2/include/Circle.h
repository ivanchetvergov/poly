// Circle.h
#pragma once

#include "Shape.h"  
#include "MyVector.h"

class Circle : public Shape {
public:
    Circle(const Vector& center, double radius);

    void Move(const Vector& v) override;
    void Out() const override;
    double Area() const override;
private:
    Vector ptCntr;
    double R;
};