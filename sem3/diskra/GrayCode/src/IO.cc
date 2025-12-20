// IO.cc
#include "IO.h"
#include "Exceptions.h"
#include <limits>
#include <iostream>
#include <string>

using std::cout;
using std::cin;

int readInteger(const std::string& data) {
    int value;
    cout << data;
    cin >> value;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidValueException("некорректный ввод. ожидалось число.");
    }

    char nextChar = cin.peek();
    if (nextChar != EOF && nextChar != '\n') {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidValueException("лишние символы после числа.");
    }

    return value;
}