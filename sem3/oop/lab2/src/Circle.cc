#include "../include/Circle.h"
#include <iostream>
#include <cmath> // M_PI

Circle::Circle(const Vector& center, double radius)
    : ptCntr(center), R(radius) {}

void Circle::Move(const Vector& v) {
    ptCntr = ptCntr + v;
}

void Circle::Out() const {
    std::cout << "\nCircle: Center = (";
    ptCntr.Out();
    std::cout << "), Radius = " << R << ", Area = " << Area();
}

double Circle::Area() const {
    return M_PI * R * R;
}