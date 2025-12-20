// func.cc
#include "../include/func.hpp"

#include <cctype>
#include <string>
#include <algorithm>


bool Pred1_1(const Point& p) {
    return (p.x >= 7 && p.x <=7) && (p.y >= 7 && p.y <=7);
}

void to_lower_case(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}