#include "TextGenerator.h"

#include <fstream>
#include <vector>

namespace dict {

TextGenerator::TextGenerator() : m_gen(std::random_device{}()), m_dist(0, 0) {}

std::string TextGenerator::generate(size_t length) {
    static std::vector<std::string> const chars = {
        "а", "б", "в", "г", "д", "е", "ё", "ж", "з", "и", "й", "к", "л", "м", "н", "о",
        "п", "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю",
        "я", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", " ", ",", "."};

    std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);

    std::string result;
    result.reserve(length * 3);

    for (size_t i = 0; i < length; ++i) {
        result += chars[dist(m_gen)];
    }

    return result;
}

}  // namespace dict
