#include "bdd.h"

#include <sstream>
#include <stdexcept>

BDDGraph::BDDGraph(std::vector<std::string> variables, std::vector<int> truthVector)
    : variables_(std::move(variables)), truth_(std::move(truthVector)) {
    if (truth_.empty() || (truth_.size() & (truth_.size() - 1)) != 0) {
        throw std::invalid_argument("Размер таблицы истинности должен быть степенью двойки");
    }
    nodes_.emplace(0, BDDNode{0, "0", static_cast<int>(variables_.size()), -1, -1, true, 0});
    nodes_.emplace(1, BDDNode{1, "1", static_cast<int>(variables_.size()), -1, -1, true, 1});
    rootId_ = build(0, 0, static_cast<int>(truth_.size()));
}

int BDDGraph::evaluate(const std::vector<int>& values) const {
    if (values.size() != variables_.size()) {
        throw std::invalid_argument("Некорректное число переменных");
    }
    return evaluateNode(rootId_, values);
}

std::string BDDGraph::describe() const {
    std::ostringstream oss;
    oss << "BDD nodes" << '\n';
    for (const auto& [id, node] : nodes_) {
        if (node.isSheet) {
            oss << id << ": leaf " << node.value << '\n';
        } else {
            oss << id << ": " << node.var
                << " | false -> " << node.zero
                << " | true -> " << node.one << '\n';
        }
    }
    oss << "root = " << rootId_ << '\n';
    return oss.str();
}

int BDDGraph::build(int level, int offset, int blockSize) {
    if (blockSize == 1) {
        return truth_[offset] ? 1 : 0;
    }
    // рекурсивное построение левой и правой частей
    const int half = blockSize / 2;
    const int low = build(level + 1, offset, half);
    const int high = build(level + 1, offset + half, half);
    if (low == high) {
        return low;
    }
    // создание или поиск уникального узла
    NodeKey key{level, low, high};
    const auto it = unique_.find(key);
    if (it != unique_.end()) {
        return it->second; // узел уже существует
    }
    const int id = nextId_++;
    // добавление нового узла 
    nodes_[id] = BDDNode{id, variables_[level], level, low, high, false, -1};
    unique_.emplace(key, id);
    return id;
}

int BDDGraph::evaluateNode(int nodeId, const std::vector<int>& values) const {
    const auto it = nodes_.find(nodeId);
    if (it == nodes_.end()) {
        throw std::runtime_error("Неизвестный узел в БДР");
    }
    const BDDNode& node = it->second;
    if (node.isSheet) {
        return node.value;
    }
    const int bit = values.at(node.level);
    const int nextId = bit ? node.one : node.zero;
    return evaluateNode(nextId, values);
}
