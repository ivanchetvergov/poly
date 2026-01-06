#pragma once

#include <random>

namespace graph {

class RandomGenerator {
public:
    RandomGenerator() : m_gen(m_rd()) {}
    explicit RandomGenerator(unsigned seed) : m_gen(seed) {}

    [[nodiscard]] int randomInt(int min, int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(m_gen);
    }

    [[nodiscard]] double randomDouble(double min, double max) {
        std::uniform_real_distribution<> dist(min, max);
        return dist(m_gen);
    }

    [[nodiscard]] std::mt19937& generator() { return m_gen; }

private:
    std::random_device m_rd;
    std::mt19937 m_gen;
};

} // namespace graph
