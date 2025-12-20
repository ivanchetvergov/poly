// MyVector.h
#pragma once

#include "Shape.h"

class Vector : public Shape {
public:
    Vector(double c1, double c2);
    Vector(const Vector& other);
    Vector();
    ~Vector() override; 
    
    Vector& operator=(const Vector& v);
    Vector operator+(const Vector& other) const;
    Vector operator*(double scalar) const;
    double operator!() const;
    bool operator<(const Vector& other) const;

    friend bool operator>(const Vector& l, const Vector& r);
    friend bool operator==(const Vector& l, const Vector& r);

    friend double operator*(const Vector& v1, const Vector& v2);

    void Move(const Vector& v) override;
    void Out() const override;
    double Area() const override;

	double GetX() const { return x; }
    double GetY() const { return y; }

private:
    double x, y;
};

Vector operator*(double scalar, const Vector& v);
