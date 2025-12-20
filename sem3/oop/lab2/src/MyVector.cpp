// MyVector.cpp
#include "../include/stdafx.h"
#include "../include/MyVector.h"

#include <cmath>
#include <cfloat>

Vector::Vector(double c1, double c2) : x(c1), y(c2) {}

Vector::Vector() : x(0.0), y(0.0) {}

Vector::Vector(const Vector& other) : x(other.x), y(other.y) {}

Vector::~Vector() {}

Vector& Vector::operator= (const Vector& v)	// Присвоение
{
	if (this == &v) return *this;
	x = v.x;
	y = v.y;
	return *this;
}

bool Vector::operator<(const Vector& other) const {
    return (x*x + y*y) < (other.x*other.x + other.y*other.y);

}

Vector Vector::operator+(const Vector& other) const{
    return Vector(x + other.x, y + other.y);
}

bool operator==(const Vector& l, const Vector& r){
	return std::fabs(l.x - r.x) < DBL_EPSILON && std::fabs(l.y - r.y) < DBL_EPSILON;
}

Vector Vector::operator*(double scalar) const {
    return Vector(x * scalar, y * scalar);
}

double Vector::operator!() const {
    return std::sqrt(x * x + y * y);
}

bool operator>(const Vector& l, const Vector& r){
	return (!l) > (!r);
}

void Vector::Out() const{
	cout << "\nVector:  x = " << x << ",  y = " << y;
}

Vector operator*(double scalar, const Vector& v) {
    return v * scalar;
}

void Vector::Move(const Vector& v) {
    x += v.x;
    y += v.y;
}

double Vector::Area() const {
    return 0.0;
}

double operator*(const Vector& v1, const Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
