#include "RBTree.h"

#include <fstream>
#include <functional>
#include <iostream>

namespace dict {

RBTree::RBTree() : m_root(nullptr) {}

RBTree::~RBTree() {
    clear();
}

void RBTree::clear() {
    clearHelper(m_root);
    m_root.reset();
}

void RBTree::clearHelper(std::unique_ptr<RBNode>& node) {
    if (!node)
        return;
    clearHelper(node->left);
    clearHelper(node->right);
    node.reset();
}

bool RBTree::insert(std::string const& word) {
    if (!m_root) {
        m_root = std::make_unique<RBNode>(word);
        m_root->color = BLACK;
        return true;
    }
    auto [parent, existingNode] = findInsertPosition(word);
    if (existingNode) {
        ++existingNode->count;
        return true;
    }
    auto newNode = std::make_unique<RBNode>(word);
    newNode->parent = parent;
    RBNode* newNodeRaw = newNode.get();

    if (word < parent->key)
        parent->left = std::move(newNode);
    else
        parent->right = std::move(newNode);

    insertFixup(newNodeRaw);
    return true;
}

bool RBTree::search(std::string const& word) const {
    RBNode* curr = m_root.get();
    while (curr) {
        if (word == curr->key)
            return true;
        if (word < curr->key)
            curr = curr->left.get();
        else
            curr = curr->right.get();
    }
    return false;
}

RBNode* RBTree::findNode(std::string const& word) const {
    RBNode* curr = m_root.get();
    while (curr) {
        if (word == curr->key)
            return curr;
        if (word < curr->key)
            curr = curr->left.get();
        else
            curr = curr->right.get();
    }
    return nullptr;
}

Color RBTree::getColor(RBNode* node) const {
    return (node == nullptr) ? BLACK : node->color;
}

bool RBTree::remove(std::string const& word) {
    RBNode* node = findNode(word);
    if (!node)
        return false;

    Color originalColor = node->color;
    RBNode* fixupNode = nullptr;

    if (!node->left && !node->right) {
        fixupNode = nullptr;
        std::unique_ptr<RBNode>& nodeRef =
            (node->parent
                 ? (node == node->parent->left.get() ? node->parent->left : node->parent->right)
                 : m_root);
        if (originalColor == BLACK) {
            deleteFixup(node);
        }
        nodeRef.reset();
    } else if (!node->left || !node->right) {
        RBNode* child = node->left ? node->left.get() : node->right.get();
        fixupNode = child;
        std::unique_ptr<RBNode>& nodeRef =
            (node->parent
                 ? (node == node->parent->left.get() ? node->parent->left : node->parent->right)
                 : m_root);
        if (node->left) {
            nodeRef = std::move(node->left);
        } else {
            nodeRef = std::move(node->right);
        }
        nodeRef->parent = node->parent;
        if (originalColor == BLACK) {
            deleteFixup(fixupNode);
        }
    } else {
        RBNode* succ = node->right.get();
        while (succ->left)
            succ = succ->left.get();
        node->key = succ->key;
        node->count = succ->count;
        originalColor = succ->color;
        remove(succ->key);
    }

    if (m_root)
        m_root->color = BLACK;
    return true;
}

std::pair<RBNode*, RBNode*> RBTree::findInsertPosition(std::string const& word) const {
    RBNode* curr = m_root.get();
    RBNode* parent = nullptr;
    while (curr) {
        parent = curr;
        if (word == curr->key)
            return {parent, curr};
        if (word < curr->key)
            curr = curr->left.get();
        else
            curr = curr->right.get();
    }
    return {parent, nullptr};
}

void RBTree::preOrderVisit(std::unique_ptr<RBNode> const& node) const {
    if (!node)
        return;
    std::cout << node->key << " (" << (node->color == RED ? "R" : "B") << ") ";
    preOrderVisit(node->left);
    preOrderVisit(node->right);
}

void RBTree::inOrderVisit(std::unique_ptr<RBNode> const& node) const {
    if (!node)
        return;
    inOrderVisit(node->left);
    std::cout << node->key << " (" << (node->color == RED ? "R" : "B") << ") ";
    inOrderVisit(node->right);
}

void RBTree::postOrderVisit(std::unique_ptr<RBNode> const& node) const {
    if (!node)
        return;
    postOrderVisit(node->left);
    postOrderVisit(node->right);
    std::cout << node->key << " (" << (node->color == RED ? "R" : "B") << ") ";
}

void RBTree::printTree() const {
    std::cout << "In-order: ";
    inOrderVisit(m_root);
    std::cout << std::endl;
}

void RBTree::deleteFixup(RBNode* pt) {
    while (pt != m_root.get() && getColor(pt) == BLACK) {
        if (!pt || !pt->parent)
            break;

        if (pt == pt->parent->left.get()) {
            RBNode* sibling = pt->parent->right.get();

            if (getColor(sibling) == RED) {
                sibling->color = BLACK;
                pt->parent->color = RED;
                rotateLeft(pt->parent);
                sibling = pt->parent->right.get();
            }

            if (getColor(sibling->left.get()) == BLACK && getColor(sibling->right.get()) == BLACK) {
                if (sibling)
                    sibling->color = RED;
                pt = pt->parent;
            } else {
                if (getColor(sibling->right.get()) == BLACK) {
                    if (sibling->left)
                        sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = pt->parent->right.get();
                }
                sibling->color = pt->parent->color;
                pt->parent->color = BLACK;
                if (sibling->right)
                    sibling->right->color = BLACK;
                rotateLeft(pt->parent);
                pt = m_root.get();
            }
        } else {
            RBNode* sibling = pt->parent->left.get();

            if (getColor(sibling) == RED) {
                sibling->color = BLACK;
                pt->parent->color = RED;
                rotateRight(pt->parent);
                sibling = pt->parent->left.get();
            }

            if (getColor(sibling->right.get()) == BLACK && getColor(sibling->left.get()) == BLACK) {
                if (sibling)
                    sibling->color = RED;
                pt = pt->parent;
            } else {
                if (getColor(sibling->left.get()) == BLACK) {
                    if (sibling->right)
                        sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = pt->parent->left.get();
                }
                sibling->color = pt->parent->color;
                pt->parent->color = BLACK;
                if (sibling->left)
                    sibling->left->color = BLACK;
                rotateRight(pt->parent);
                pt = m_root.get();
            }
        }
    }
    if (pt)
        pt->color = BLACK;
}

void RBTree::insertFixup(RBNode* pt) {
    while (pt != m_root.get() && pt->parent && pt->parent->color == RED) {
        RBNode* parent = pt->parent;
        RBNode* grandparent = parent->parent;
        if (!grandparent)
            break;

        if (parent == grandparent->left.get()) {
            RBNode* uncle = grandparent->right.get();
            if (uncle && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                pt = grandparent;
            } else {
                if (pt == parent->right.get()) {
                    pt = parent;
                    rotateLeft(pt);
                    parent = pt->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rotateRight(grandparent);
            }
        } else {
            RBNode* uncle = grandparent->left.get();
            if (uncle && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                pt = grandparent;
            } else {
                if (pt == parent->left.get()) {
                    pt = parent;
                    rotateRight(pt);
                    parent = pt->parent;
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rotateLeft(grandparent);
            }
        }
    }
    m_root->color = BLACK;
}

void RBTree::rotateLeft(RBNode* pt) {
    if (!pt || !pt->right)
        return;

    std::unique_ptr<RBNode> pt_right = std::move(pt->right);

    pt->right = std::move(pt_right->left);
    if (pt->right)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (!pt->parent) {
        m_root = std::move(pt_right);
        m_root->left = std::unique_ptr<RBNode>(pt);
        m_root->left->parent = m_root.get();
    } else if (pt == pt->parent->left.get()) {
        pt->parent->left = std::move(pt_right);
        pt->parent->left->left = std::unique_ptr<RBNode>(pt);
        pt->parent->left->left->parent = pt->parent->left.get();
    } else {
        pt->parent->right = std::move(pt_right);
        pt->parent->right->left = std::unique_ptr<RBNode>(pt);
        pt->parent->right->left->parent = pt->parent->right.get();
    }
    pt->parent = (pt->parent ? (pt == pt->parent->left.get() ? pt->parent->left.get()
                                                             : pt->parent->right.get())
                             : m_root.get());
}

void RBTree::rotateRight(RBNode* pt) {
    if (!pt || !pt->left)
        return;

    std::unique_ptr<RBNode> pt_left = std::move(pt->left);

    pt->left = std::move(pt_left->right);
    if (pt->left)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (!pt->parent) {
        m_root = std::move(pt_left);
        m_root->right = std::unique_ptr<RBNode>(pt);
        m_root->right->parent = m_root.get();
    } else if (pt == pt->parent->left.get()) {
        pt->parent->left = std::move(pt_left);
        pt->parent->left->right = std::unique_ptr<RBNode>(pt);
        pt->parent->left->right->parent = pt->parent->left.get();
    } else {
        pt->parent->right = std::move(pt_left);
        pt->parent->right->right = std::unique_ptr<RBNode>(pt);
        pt->parent->right->right->parent = pt->parent->right.get();
    }
    pt->parent = (pt->parent ? (pt == pt->parent->left.get() ? pt->parent->left.get()
                                                             : pt->parent->right.get())
                             : m_root.get());
}

bool RBTree::loadFromFile(std::string const& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string content;
    std::string word;

    // Читаем весь файл
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();

    // Разбиваем на слова по разделителям
    for (char c : content) {
        if (c == ' ' || c == ',' || c == '.' || c == '\n' || c == '\r' || c == '\t') {
            if (!word.empty()) {
                insert(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        insert(word);
    }

    return true;
}

bool RBTree::saveToFile(std::string const& filename) const {
    std::ofstream file(filename);
    if (!file.is_open())
        return false;
    std::function<void(std::unique_ptr<RBNode> const&)> saveInOrder =
        [&](std::unique_ptr<RBNode> const& node) {
            if (!node)
                return;
            saveInOrder(node->left);
            file << node->key << " " << node->count << "\n";
            saveInOrder(node->right);
        };
    saveInOrder(m_root);
    file.close();
    return true;
}

}  // namespace dict