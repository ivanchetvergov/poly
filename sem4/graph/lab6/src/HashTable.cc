#include "HashTable.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace dict {

HashTable::HashTable(size_t capacity) : m_capacity(capacity), m_size_(0) {
    m_table.resize(m_capacity);
}

HashTable::~HashTable() {
    clear();
}

bool HashTable::insert(std::string const& word) {
    size_t idx = hash(word);
    HashNode* curr = m_table[idx].get();
    if (!curr) {
        m_table[idx] = std::make_unique<HashNode>(word);
        ++m_size_;
        return true;
    }
    while (curr) {
        if (curr->key == word) {
            ++curr->count;
            return true;
        }
        if (!curr->next) {
            curr->next = std::make_unique<HashNode>(word);
            ++m_size_;
            return true;
        }
        curr = curr->next.get();
    }
    return false;
}

bool HashTable::remove(std::string const& word) {
    size_t idx = hash(word);
    auto* curr = m_table[idx].get();
    if (!curr)
        return false;
    if (curr->key == word) {
        m_table[idx] = std::move(curr->next);
        --m_size_;
        return true;
    }
    HashNode* prev = curr;
    curr = curr->next.get();
    while (curr) {
        if (curr->key == word) {
            prev->next = std::move(curr->next);
            --m_size_;
            return true;
        }
        prev = curr;
        curr = curr->next.get();
    }
    return false;
}

bool HashTable::search(std::string const& word) const {
    size_t idx = hash(word);
    auto* curr = m_table[idx].get();
    while (curr) {
        if (curr->key == word)
            return true;
        curr = curr->next.get();
    }
    return false;
}

void HashTable::clear() {
    for (size_t i = 0; i < m_capacity; ++i) {
        m_table[i].reset();
    }
    m_size_ = 0;
}

size_t HashTable::hash(std::string const& key) const noexcept {
    // * DJB2 hash function : hash(i) = hash(i - 1) * 33 + c
    size_t hash = 5381;

    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<size_t>(c);
    }
    return hash % m_capacity;
}

bool HashTable::loadFromFile(std::string const& filename) {
    std::vector<std::pair<std::string, int>> pairs;
    if (!FileHandler::loadKeyValuePairs(filename, pairs)) return false;
    clear();
    for (auto const& [key, count] : pairs) {
        for (int i = 0; i < count; ++i) {
            insert(key);
        }
    }
    return true;
}

bool HashTable::saveToFile(std::string const& filename) const {
    std::vector<std::pair<std::string, int>> pairs;
    for (size_t i = 0; i < m_capacity; ++i) {
        auto* curr = m_table[i].get();
        while (curr) {
            pairs.emplace_back(curr->key, curr->count);
            curr = curr->next.get();
        }
    }
    return FileHandler::saveKeyValuePairs(filename, pairs);
}

void HashTable::printTable() const {
    std::cout << "\n=== Содержимое хеш-таблицы ===" << std::endl;
    std::cout << "Размер: " << m_size_ << ", Емкость: " << m_capacity << std::endl;
    std::cout << "Слова:" << std::endl;

    int count = 0;
    for (size_t i = 0; i < m_capacity; ++i) {
        auto* curr = m_table[i].get();
        while (curr) {
            std::cout << "  " << curr->key << " (" << curr->count << "x)" << std::endl;
            curr = curr->next.get();
            count++;
        }
    }

    if (count == 0) {
        std::cout << "  (пусто)" << std::endl;
    }
}

bool HashTable::exportForVisualization(std::string const& filename) const {
    std::string content;
    for (size_t i = 0; i < m_capacity; ++i) {
        auto* curr = m_table[i].get();
        if (!curr) continue;
        content += std::to_string(i) + " ";  // bucket_index
        while (curr) {
            content += curr->key + " " + std::to_string(curr->count) + " ";
            curr = curr->next.get();
        }
        content += "\n";
    }
    return FileHandler::saveToFile(filename, content);
}

}  // namespace dict
