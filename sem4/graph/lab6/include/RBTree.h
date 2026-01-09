#pragma once
#include <memory>
#include <string>

namespace dict {

enum Color { RED, BLACK };

struct RBNode {
    std::string key;
    int count;
    Color color{RED};
    std::unique_ptr<RBNode> left{nullptr};
    std::unique_ptr<RBNode> right{nullptr};
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

    void printTree() const;

    bool loadFromFile(std::string const& filename);
    bool saveToFile(std::string const& filename) const;

private:
    std::unique_ptr<RBNode> m_root;

    RBNode* findNode(std::string const& word) const;
    Color getColor(RBNode* node) const;
    std::pair<RBNode*, RBNode*> findInsertPosition(std::string const& word) const;
    void clearHelper(std::unique_ptr<RBNode>& node);

    void preOrderVisit(std::unique_ptr<RBNode> const& node) const;
    void inOrderVisit(std::unique_ptr<RBNode> const& node) const;
    void postOrderVisit(std::unique_ptr<RBNode> const& node) const;

    void rotateLeft(RBNode* pt);
    void rotateRight(RBNode* pt);
    void insertFixup(RBNode* pt);
    void deleteFixup(RBNode* pt);
};

}  // namespace dict
