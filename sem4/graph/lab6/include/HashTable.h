#pragma once
#include <string>
#include <memory>
#include <vector>

namespace dict {

struct HashNode {
    std::string key;
    int count;
    std::unique_ptr<HashNode> next;

    HashNode(const std::string& k, int c = 1) 
        : key(k), count(c), next(nullptr) {}
};

class HashTable {
public: 
    explicit HashTable(size_t capacity = 1024);
    ~HashTable();

    bool insert(const std::string& word);
    bool remove(const std::string& word);
    bool search(const std::string& word) const;
    void clear();

    [[nodiscard]] size_t size() const noexcept { return m_size; }
    [[nodiscard]] size_t capacity() const noexcept { return m_capacity; }
    void printTable() const;

    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const; 

private:
    std::vector<std::unique_ptr<HashNode>> m_table;
    size_t m_capacity;
    size_t m_size;

    [[nodiscard]] size_t hash(const std::string& key) const noexcept;
};

} // namespace dict