#include "HashTable.h"
#include "FileHandler.h"

#include <fstream>
#include <iostream>
#include <sstream>

using graph::FileHandler;

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
    for (auto& bucket : m_table) {
        bucket.reset();
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

std::string HashTable::serialize() const {
    std::string content;
    for (size_t i = 0; i < m_capacity; ++i) {
        auto* curr = m_table[i].get();
        while (curr) {
            size_t h = hash(curr->key);
            content += curr->key + " " + std::to_string(h) + " " + std::to_string(i) + "\n";
            curr = curr->next.get();
        }
    }
    return content;
}

}  // namespace dict
