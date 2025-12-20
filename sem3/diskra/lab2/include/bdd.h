#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

struct BDDNode {
    int id{-1};
    std::string var;
    int level{-1};
    int zero{-1};
    int one{-1};
    bool isSheet{false};
    int value{-1};
};

class BDDGraph {
public:
    BDDGraph(std::vector<std::string> variables, std::vector<int> truthVector);

    int evaluate(const std::vector<int>& values) const;
    std::string describe() const;

private:
    struct NodeKey {
        int level;
        int low;
        int high;

        bool operator==(const NodeKey& other) const {
            return level == other.level && low == other.low && high == other.high;
        }
    };

    struct NodeKeyHash {
        size_t operator()(const NodeKey& key) const {
            size_t h = static_cast<size_t>(key.level);
            h = h * 1315423911u + static_cast<size_t>(key.low);
            h = h * 2654435761u + static_cast<size_t>(key.high);
            return h;
        }
    };

    std::vector<std::string> variables_;
    std::vector<int> truth_;
    std::map<int, BDDNode> nodes_;
    std::unordered_map<NodeKey, int, NodeKeyHash> unique_;
    int rootId_{-1};
    int nextId_{2};

    int build(int level, int offset, int blockSize);
    int evaluateNode(int nodeId, const std::vector<int>& values) const;
};
