#pragma once
#include <memory>
#include <string>

#include "../../common/include/FileHandler.h"

namespace dict {

enum Color { RED, BLACK };

struct RBNode {
    std::string key;
    int count;
    Color color{RED};
    RBNode* left{nullptr};
    RBNode* right{nullptr};
    RBNode* parent{nullptr};

    explicit RBNode(std::string const& k, int c = 1) : key(k), count(c) {}
};

class RBTree {
public:
    explicit RBTree();
    ~RBTree();

    bool insert(std::string const& word);
    bool remove(std::string const& word);
    bool search(std::string const& word) const;
    void clear();

    [[nodiscard]] std::string serialize() const;

    bool saveToFile(std::string const& filename) const;

    bool loadFromFile(std::string const& filename);

    bool validate() const;

private:
    RBNode* m_root;

    RBNode* findNode(std::string const& word) const;
    Color getColor(RBNode* node) const;
    std::pair<RBNode*, RBNode*> findInsertPosition(std::string const& word) const;
    void destroy(RBNode* node);


    void rotateLeft(RBNode* pt);
    void rotateRight(RBNode* pt);
    void insertFixup(RBNode* pt);
    void deleteFixup(RBNode* pt);

    RBNode* minimum(RBNode* node) const;
    void transplant(RBNode* u, RBNode* v);
};

}  // namespace dict
