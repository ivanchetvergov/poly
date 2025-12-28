#include "HashTable.h"
#include <fstream>
#include <sstream>

namespace dict {


HashTable::HashTable(size_t capacity)
    : m_capacity(capacity), m_size(0) {
    m_table.resize(m_capacity);
}

HashTable::~HashTable() {
    clear();
}

bool HashTable::insert(const std::string& word) {
    size_t idx = hash(word);
    HashNode* curr = m_table[idx].get();
    if (!curr) {
        m_table[idx] = std::make_unique<HashNode>(word);
        ++m_size;
        return true;
    }
    while (curr) {
        if (curr->key == word) {
            ++curr->count;
            return true;
        }
        if (!curr->next) {
            curr->next = std::make_unique<HashNode>(word);
            ++m_size;
            return true;
        }
        curr = curr->next.get();
    }
    return false;
}
 
bool HashTable::remove(const std::string& word) {
    size_t idx = hash(word);
    auto* curr = m_table[idx].get();
    if (!curr) return false;
    if (curr->key == word) {
        m_table[idx] = std::move(curr->next);
        --m_size;
        return true;
    }
    HashNode* prev = curr;
    curr = curr->next.get();
    while (curr) {
        if (curr->key == word) {
            prev->next = std::move(curr->next);
            --m_size;
            return true;
        }
        prev = curr;
        curr = curr->next.get();
    }
    return false;
}
    
bool HashTable::search(const std::string& word) const {
    size_t idx = hash(word);
    auto* curr = m_table[idx].get();
    while (curr) {
        if (curr->key == word) return true;
        curr = curr->next.get();
    }
    return false;
}

void HashTable::clear() {
    for (size_t i = 0; i < m_capacity; ++i) {
        m_table[i].reset();
    }
    m_size = 0;
}

        
size_t HashTable::hash(const std::string& key) const noexcept {
    // * DJB2 hash function : hash(i) = hash(i - 1) * 33 + c
    size_t hash = 5381;
        
    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<size_t>(c); 
    }
    return hash % m_capacity;
}

bool HashTable::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    while (file >> line) {
        insert(line);
    }
    file.close();
    return true;
}

bool HashTable::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (size_t i = 0; i < m_capacity; ++i) {
        auto* curr = m_table[i].get();
        while (curr) {
            file << curr->key << " " << curr->count << "\n";
            curr = curr->next.get();
        }
    }
    file.close();
    return true;
}

} // namespace dict


