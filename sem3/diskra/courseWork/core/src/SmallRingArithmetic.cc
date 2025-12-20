// core/src/SmallRingArithmetic.cc
#include "SmallRingArithmetic.h"
#include <stdexcept>
#include <stack>
#include <utility>

char SmallRingArithmetic::plusOne(char c) const {
    // 1. получаем упорядоченный список символов кольца
    const std::vector<char>& values = rules_.getOrderedValues(); 
    
    // 2. находим позицию текущего символа c (O(N))
    auto it = std::find(values.begin(), values.end(), c);

    // проверка на ошибку
    if (it == values.end()) {
        throw std::runtime_error("Invalid ring character found.");
    }

    // 3. продвигаем итератор на одну позицию
    auto next_it = std::next(it);

    // 4. проверка цикличности: если достигли конца, возвращаемся к началу
    if (next_it == values.end()) {
        // циклический переход с N-1 на 0
        return values.front();
    }

    // 5. возвращаем следующий элемент
    return *next_it;
}

char SmallRingArithmetic::add(char a, char b) const {
    const char zero = rules_.getZeroElement();
    
    if (b == zero) {
        return a;
    }

    char counter = zero;
    char result = a;
    
    // итерация b раз, используя char как счетчик
    while (counter != b) {
        result = plusOne(result);
        counter = plusOne(counter); //  продвигаем счетчик
    }
    return result;
}


char SmallRingArithmetic::subtract(char a, char b) const {
    // ? вариант разницы индексов O(1)
    // int va = rules_.getCharValue(a);
    // int vb = rules_.getCharValue(b);
    // int n = rules_.getSize();
    
    // // Вычитаем с учетом модуля
    // int result_index = (va - vb + n) % n;
    
    // return rules_.getValueChar(result_index);

    // ! вариант через сложение с обратным (N))
    char neg_b = findAdditiveInverse(b); 
    return add(a, neg_b);
}

char SmallRingArithmetic::multiply(char a, char b) const {
    const char zero = rules_.getZeroElement();
    const char one = rules_.getOneElement();
    
    if (a == zero || b == zero) return zero;
    
    // используем b как счетчик, уменьшая его до нуля.
    char result = zero;
    char counter = b;
    
    while (counter != zero) {
        // result = result + a
        result = add(result, a); 
        
        // counter = counter - 1
        counter = subtract(counter, one); 
    }
    
    return result;
}

char SmallRingArithmetic::divide(char a, char b) const {
    const char zero = rules_.getZeroElement();
    
    // деление на ноль
    if (b == zero) {
        throw std::runtime_error("Division by zero");
    }
    
    // находим обратный элемент
    char inv = findMultiplicativeInverse(b);
    return multiply(a, inv);
}

char SmallRingArithmetic::findAdditiveInverse(char element) const {
    const char zero = rules_.getZeroElement();
    
    if (element == zero) {
        return zero;
    }
    
    char candidate = zero;
    do {
        char sum = add(element, candidate);
        if (sum == zero) {
            return candidate;
        }
        candidate = plusOne(candidate);
    } while (candidate != zero);
    
    throw std::runtime_error("Additive inverse logic error: Full circle without result."); 
}

char SmallRingArithmetic::findMultiplicativeInverse(char element) const {
    const char zero = rules_.getZeroElement();
    const char one = rules_.getOneElement();
    
    if (element == zero) {
        throw std::runtime_error("Zero has no multiplicative inverse");
    }

    char candidate = one;
    do {
        char product = multiply(element, candidate);
        if (product == one) {
            return candidate;
        }
        candidate = plusOne(candidate);
    } while (candidate != one);
    
    throw std::runtime_error("No multiplicative inverse for element: " + 
                           std::string(1, element));
}