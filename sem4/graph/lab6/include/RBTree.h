#pragma once
#include <string>
#include <memory>

namespace dict {

enum Color { RED, BLACK };

struct RBNode {
    std::string key;
    int count;
    Color color {RED};
    std::unique_ptr<RBNode> left {nullptr};
    std::unique_ptr<RBNode> right {nullptr};
    RBNode* parent {nullptr};
    
    RBNode(const std::string& k, int c = 1)
        : key(k), count(c) {}
};

class RBTree {
public:
    explicit RBTree();
    ~RBTree();

    bool insert(const std::string& word);
    bool remove(const std::string& word);
    bool search(const std::string& word) const;
    void clear();

    void printTree() const;
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

private:
    std::unique_ptr<RBNode> m_root;

    RBNode* findNode(const std::string& word) const;
    Color getColor(RBNode* node) const;
    std::pair<RBNode*, RBNode*> findInsertPosition(const std::string& word) const;
    void clearHelper(std::unique_ptr<RBNode>& node);

    void preOrderVisit(const std::unique_ptr<RBNode>& node) const;
    void inOrderVisit(const std::unique_ptr<RBNode>& node) const;
    void postOrderVisit(const std::unique_ptr<RBNode>& node) const;

    void rotateLeft(RBNode* pt);
    void rotateRight(RBNode* pt);
    void insertFixup(RBNode* pt);
    void deleteFixup(RBNode* pt);
};


} // namespace dict