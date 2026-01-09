#pragma once
#include <random>
#include <string>

namespace dict {

class TextGenerator {
public:
    TextGenerator();

    std::string generate(size_t length = 10'000);
    bool saveToFile(std::string const& filename, size_t length);

private:
    std::mt19937 m_gen;
    std::uniform_int_distribution<int> m_dist;

    char getRandomChar();
};

}  // namespace dict
