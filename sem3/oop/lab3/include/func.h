// temp.h

#pragma once

#include <iostream> 
#include <list>      
#include <string>

template <class T>
void Swap(T& x, T& y);

void printList(const std::list<Vector>& lst, const std::string& title) {
    std::cout << "\n\n\t" << title << "\n";
    int i = 0;
    for (const auto& v : lst) {
        std::cout << "\n" << ++i << ". ";
        v.Out();
    }
    std::cout << '\n';
}

bool bothCoordsGreaterThanTwo(const Vector& v) {
    return (v.GetX() > 2.0) && (v.GetY() > 2.0);
}

template <typename T>
void printContainer(const T& cont);

template <typename T>
void printPop(T container);

template <typename T>
void printMap(const T& container);

#include "../tpl/func.tpp" 