#pragma once
#include <memory>
#include <string>
#include <vector>

#include "FileHandler.h"

namespace dict {

struct HashNode {
    std::string key;
    int count;
    std::unique_ptr<HashNode> next;

    explicit HashNode(std::string const& k, int c = 1) : key(k), count(c), next(nullptr) {}
};

class HashTable {
public:
    explicit HashTable(size_t capacity = 1024);
    ~HashTable();

    bool insert(std::string const& word);
    bool remove(std::string const& word);
    bool search(std::string const& word) const;
    void clear();

    [[nodiscard]] size_t size() const noexcept { return m_size_; }
    [[nodiscard]] size_t capacity() const noexcept { return m_capacity; }
    void printTable() const;

    bool exportForVisualization(std::string const& filename) const;

    bool loadFromFile(std::string const& filename);
    bool saveToFile(std::string const& filename) const;

private:
    std::vector<std::unique_ptr<HashNode>> m_table;
    size_t m_capacity;
    size_t m_size_;

    [[nodiscard]] size_t hash(std::string const& key) const noexcept;
};

}  // namespace dict
