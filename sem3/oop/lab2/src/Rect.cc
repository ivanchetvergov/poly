// Rect.cc
#include "../include/Rect.h"
#include <iostream>

Rect::Rect() : ptLT_(0.0, 0.0), ptRB_(0.0, 0.0) {}

Rect::Rect(const Rect& other) : ptLT_(other.ptLT_), ptRB_(other.ptRB_) {}

Rect::Rect(double left, double top, double right, double bottom)
    : ptLT_(left, top), ptRB_(right, bottom) {}

Rect::~Rect(){
    // std::cout << "\nвызван ~Rect" << std::endl;
}

Rect::Rect(const Vector& lt, const Vector& rb)
    : ptLT_(lt), ptRB_(rb) {}

Rect& Rect::operator=(const Rect& other) {
    if (this != &other) {
        ptLT_ = other.ptLT_;
        ptRB_ = other.ptRB_;
    }
    return *this;
}

void Rect::Inflate(double value) {
    ptLT_ = ptLT_ + Vector(-value, value);
    ptRB_ = ptRB_ + Vector(value, -value);
}


void Rect::Inflate(double x_value, double y_value) {
    ptLT_ = ptLT_ + Vector(-x_value, y_value);
    ptRB_ = ptRB_ + Vector(x_value, -y_value);
}


void Rect::Inflate(double left, double top, double right, double bottom) {
    ptLT_ = ptLT_ + Vector(-left, top);
    ptRB_ = ptRB_ + Vector(right, -bottom);
}


void Rect::Move(const Vector& v) {
    ptLT_ = ptLT_ + v;
    ptRB_ = ptRB_ + v;
}

void Rect::Out() const {
    std::cout << "Rect: ";
    ptLT_.Out();
    ptRB_.Out();
    std::cout << std::endl;
}


double Rect::Area() const {
    double width = ptRB_.GetX() - ptLT_.GetY();
    double height = ptRB_.GetX() - ptLT_.GetY(); 
    return std::fabs(width * height);
}

Vector Rect::getCenter() const {
    double center_x = (ptLT_.GetX() + ptRB_.GetX()) / 2.0;
    double center_y = (ptLT_.GetY() + ptRB_.GetY()) / 2.0;
    return Vector(center_x, center_y);
}