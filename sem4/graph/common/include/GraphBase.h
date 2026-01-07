#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace graph {


template<typename VertexT, typename EdgeT>
class GraphBase {
public:
    GraphBase(bool isDirected = false) : isDirected_(isDirected) {}
    virtual ~GraphBase() = default;

    bool addVertex(int id);
    [[nodiscard]] bool hasVertex(int id) const noexcept;
    [[nodiscard]] std::optional<const VertexT*> getVertex(int id) const;
    [[nodiscard]] size_t vertexCount() const noexcept;
    [[nodiscard]] std::vector<int> vertexIds() const;

    bool addEdge(int from, int to, const EdgeT& edge);
    [[nodiscard]] bool hasEdge(int from, int to) const;
    [[nodiscard]] std::optional<EdgeT> getEdge(int from, int to) const;
    EdgeT* getEdgeMutable(int from, int to);
    [[nodiscard]] size_t edgeCount() const noexcept;
    [[nodiscard]] std::vector<EdgeT> edges() const;

    [[nodiscard]] std::vector<int> neighbors(int id) const;
    [[nodiscard]] int degree(int v) const;
    [[nodiscard]] bool isDirected() const noexcept { return isDirected_; }

protected:
    std::unordered_map<int, std::unique_ptr<VertexT>> m_vertices;
    std::unordered_map<long long, EdgeT> m_edges;
    bool isDirected_ = false;

    long long makeKey(int from, int to) const;
};

} // namespace graph

#include "../src/GraphBase.tpp"
