#include "RBTree.h"
#include "FileHandler.h"

#include <fstream>
#include <functional>
#include <iostream>

using graph::FileHandler;

namespace dict {

void DEBUG(const std::string& msg) {
    std::cout << "[DEBUG] " << msg << std::endl;
}

RBTree::RBTree() : m_root(nullptr) {}

RBTree::~RBTree() {
    destroy(m_root);
}

void RBTree::destroy(RBNode* node) {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

bool RBTree::insert(std::string const& word) {
    DEBUG("Inserting: " + word);
    if (!m_root) {
        m_root = new RBNode(word);
        m_root->color = BLACK;
        DEBUG("Root created: " + word);
        return true;
    }
    auto [parent, existingNode] = findInsertPosition(word);
    if (existingNode) {
        ++existingNode->count;
        DEBUG("Incremented count for: " + word);
        return true;
    }
    DEBUG("Creating new node: " + word);
    RBNode* newNode = new RBNode(word);
    newNode->parent = parent;

    if (word < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    DEBUG("Inserted: " + word + " under " + parent->key);
    insertFixup(newNode);
    DEBUG("Fixup done for: " + word);
    return true;
}

bool RBTree::search(std::string const& word) const {
    RBNode* curr = m_root;
    while (curr) {
        if (word == curr->key)
            return true;
        if (word < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return false;
}

RBNode* RBTree::findNode(std::string const& word) const {
    RBNode* curr = m_root;
    while (curr) {
        if (word == curr->key)
            return curr;
        if (word < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nullptr;
}

Color RBTree::getColor(RBNode* node) const {
    return (node == nullptr) ? BLACK : node->color;
}

RBNode* RBTree::minimum(RBNode* node) const {
    while (node && node->left)
        node = node->left;
    return node;
}

void RBTree::transplant(RBNode* u, RBNode* v) {
    if (!u->parent) {
        m_root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) v->parent = u->parent;
}

bool RBTree::remove(std::string const& word) {
    RBNode* node = findNode(word);
    if (!node)
        return false;

    RBNode* y = node;
    RBNode* x;
    Color originalColor = y->color;

    if (!node->left) {
        x = node->right;
        transplant(node, node->right);
    } else if (!node->right) {
        x = node->left;
        transplant(node, node->left);
    } else {
        y = minimum(node->right);
        originalColor = y->color;
        x = y->right;
        if (y->parent == node) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    if (originalColor == BLACK) {
        deleteFixup(x);
    }

    delete node;
    return true;
}

std::pair<RBNode*, RBNode*> RBTree::findInsertPosition(std::string const& word) const {
    RBNode* curr = m_root;
    RBNode* parent = nullptr;
    while (curr) {
        parent = curr;
        if (word == curr->key)
            return {parent, curr};
        if (word < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return {parent, nullptr};
}

std::string RBTree::serialize() const {
    std::string content;
    std::function<void(RBNode const*)> collect = [&](RBNode const* node) {
        if (!node) return;
        std::string parent = node->parent ? node->parent->key : "null";
        std::string color = node->color == RED ? "RED" : "BLACK";
        content += node->key + " " + parent + " " + color + "\n";
        collect(node->left);
        collect(node->right);
    };
    collect(m_root);
    return content;
}

void RBTree::deleteFixup(RBNode* pt) {
    while (pt != m_root && getColor(pt) == BLACK) {
        if (!pt || !pt->parent)
            break;

        if (pt == pt->parent->left) {
            RBNode* sibling = pt->parent->right;

            if (getColor(sibling) == RED) {
                sibling->color = BLACK;
                pt->parent->color = RED;
                rotateLeft(pt->parent);
                sibling = pt->parent->right;
            }

            if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                if (sibling)
                    sibling->color = RED;
                pt = pt->parent;
            } else {
                if (getColor(sibling->right) == BLACK) {
                    if (sibling->left)
                        sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = pt->parent->right;
                }
                sibling->color = pt->parent->color;
                pt->parent->color = BLACK;
                if (sibling->right)
                    sibling->right->color = BLACK;
                rotateLeft(pt->parent);
                pt = m_root;
            }
        } else {
            RBNode* sibling = pt->parent->left;

            if (getColor(sibling) == RED) {
                sibling->color = BLACK;
                pt->parent->color = RED;
                rotateRight(pt->parent);
                sibling = pt->parent->left;
            }

            if (getColor(sibling->right) == BLACK && getColor(sibling->left) == BLACK) {
                if (sibling)
                    sibling->color = RED;
                pt = pt->parent;
            } else {
                if (getColor(sibling->left) == BLACK) {
                    if (sibling->right)
                        sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = pt->parent->left;
                }
                sibling->color = pt->parent->color;
                pt->parent->color = BLACK;
                if (sibling->left)
                    sibling->left->color = BLACK;
                rotateRight(pt->parent);
                pt = m_root;
            }
        }
    }
    if (pt)
        pt->color = BLACK;
}

void RBTree::insertFixup(RBNode* pt) {
    DEBUG("Starting insertFixup on: " + pt->key);
    while (pt != m_root && pt->parent && pt->parent->color == RED) {
        RBNode* parent = pt->parent;
        RBNode* grandparent = parent->parent;
        if (!grandparent)
            break;

        DEBUG("Fixup loop: pt=" + pt->key + ", parent=" + parent->key + ", gp=" + grandparent->key);
        if (parent == grandparent->left) {
            RBNode* uncle = grandparent->right;
            if (uncle && uncle->color == RED) {
                DEBUG("Case 1: uncle red, recoloring");
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                pt = grandparent;
            } else {
                if (pt == parent->right) {
                    DEBUG("Case 2: pt is right child, rotating left on parent");
                    pt = parent;
                    rotateLeft(pt);
                    parent = pt->parent;
                }
                DEBUG("Case 3: rotating right on gp");
                parent->color = BLACK;
                grandparent->color = RED;
                rotateRight(grandparent);
            }
        } else {
            RBNode* uncle = grandparent->left;
            if (uncle && uncle->color == RED) {
                DEBUG("Case 1 mirror: uncle red, recoloring");
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                pt = grandparent;
            } else {
                if (pt == parent->left) {
                    DEBUG("Case 2 mirror: pt is left child, rotating right on parent");
                    pt = parent;
                    rotateRight(pt);
                    parent = pt->parent;
                }
                DEBUG("Case 3 mirror: rotating left on gp");
                parent->color = BLACK;
                grandparent->color = RED;
                rotateLeft(grandparent);
            }
        }
    }
    if (m_root) m_root->color = BLACK;
    DEBUG("InsertFixup done");
}

void RBTree::rotateLeft(RBNode* x) {
    DEBUG("Rotating left on: " + x->key);
    RBNode* y = x->right;
    if (!y) return;

    x->right = y->left;
    if (y->left) y->left->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        m_root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
    DEBUG("Rotate left done");
}

void RBTree::rotateRight(RBNode* x) {
    DEBUG("Rotating right on: " + x->key);
    RBNode* y = x->left;
    if (!y) return;

    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        m_root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
    DEBUG("Rotate right done");
}

bool RBTree::validate() const {
    if (!m_root) return true;
    if (m_root->color != BLACK) return false;
    std::function<int(RBNode const*)> check = [&](RBNode const* node) -> int {
        if (!node) return 1;
        if (node->color == RED) {
            if ((node->left && node->left->color == RED) || (node->right && node->right->color == RED)) return -1;
        }
        int lh = check(node->left);
        int rh = check(node->right);
        if (lh == -1 || rh == -1 || lh != rh) return -1;
        return lh + (node->color == BLACK ? 1 : 0);
    };
    return check(m_root) != -1;
}

}  // namespace dict
