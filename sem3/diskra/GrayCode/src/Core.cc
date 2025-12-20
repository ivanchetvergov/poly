// Core.cc
#include "Core.h"
#include "Exceptions.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

std::string toBinary(int number, int length) {
    if (number < 0) {
        throw InvalidValueException("отрицательное число недопустимо \
            для перевода в бинарный код.");
    }
    if (length < 0) {
        throw InvalidValueException("отрицательная длина бинарной  \
            строки недопустима.");
    }
    std::string binaryString = "";
    for (int i = length - 1; i >= 0; --i) {
        binaryString += ((number >> i) & 1) ? '1' : '0';
    }
    return binaryString;
}

std::vector<std::string> generateGrayCode(int n) {
    if (n < 0) {
        throw InvalidValueException("отрицательная разрядность \
            недопустима для генерации кода Грея.");
    }
    
    if (n > 30) { 
        throw std::out_of_range("разрядность слишком велика, в \
            озможна нехватка памяти.");
    }

    if (n == 0) {
        return {}; // исправлено с {""};
    }

    std::vector<std::string> grayCodes;
    int totalCodes = 1 << n; // 2^n
    
    grayCodes.reserve(totalCodes); // резервируем память
    
    for (int i = 0; i < totalCodes; ++i) {
        int gray_i = i ^ (i >> 1);
        grayCodes.push_back(toBinary(gray_i, n));
    }
    
    return grayCodes;
}