#include "TextGenerator.h"
#include <fstream>

namespace dict {

TextGenerator::TextGenerator() 
    : m_gen(std::random_device{}()), 
      m_dist(0, 0) {} 

char TextGenerator::getRandomChar() {
    static const std::string chars = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя0123456789 ,.";
    
    std::uniform_int_distribution<int> dist(0, chars.length() - 1);
    int index = dist(m_gen);
    
    return chars[index];
}

std::string TextGenerator::generate(size_t length) {
    std::string result;
    result.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        result += getRandomChar();
    }
    
    return result;
}

bool TextGenerator::saveToFile(const std::string& filename, size_t length) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    std::string text = generate(length);
    file << text;
    file.close();
    
    return true;
}

} // namespace dict
