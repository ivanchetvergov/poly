// func.hpp
#pragma once
#include <iostream>

// костанты для предиката 
constexpr int N = 5;
constexpr int M = 5;

struct Point {
    int x;
    int y;

    Point(int x_val, int y_val) : x(x_val), y(y_val) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class PointTransformer {
public:
    PointTransformer(int dx, int dy) : m_dx(dx), m_dy(dy) {}
    void operator()(Point& p) {
        p.x += m_dx;
        p.y += m_dy;
    }
private:
    int m_dx, m_dy;
};

template <typename Container>
void printContainer(const Container& cont);

template <typename T>
void printPop(T container);

template <typename T>
void printMap(const T& container);

template<typename T>
void print(const T& item);

bool Pred1_1(const Point& p);
void to_lower_case(std::string& s);

#include "../tpl/func.tpp"