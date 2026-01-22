#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace graph {

template <typename VertexT, typename EdgeT>
class GraphBase {
public:
    explicit GraphBase(bool isDirected = false) : is_directed_(isDirected) {}
    virtual ~GraphBase() = default;

    GraphBase(GraphBase const&) = delete;
    GraphBase& operator=(GraphBase const&) = delete;

    bool addVertex(int id);
    [[nodiscard]] bool hasVertex(int id) const noexcept;
    [[nodiscard]] std::optional<VertexT const*> getVertex(int id) const;
    [[nodiscard]] size_t vertexCount() const noexcept;
    [[nodiscard]] std::vector<int> vertexIds() const;

    bool addEdge(int from, int to, EdgeT const& edge);
    [[nodiscard]] bool hasEdge(int from, int to) const;
    [[nodiscard]] std::optional<EdgeT> getEdge(int from, int to) const;
    EdgeT* getEdgeMutable(int from, int to);
    [[nodiscard]] size_t edgeCount() const noexcept;
    [[nodiscard]] std::vector<EdgeT> edges() const;

    [[nodiscard]] std::vector<int> neighbors(int id) const;
    [[nodiscard]] int degree(int v) const;
    [[nodiscard]] bool isDirected() const noexcept { return is_directed_; }

protected:
    std::unordered_map<uint64_t, std::unique_ptr<VertexT>> m_vertices_;
    std::unordered_map<uint64_t, EdgeT> m_edges_;
    bool is_directed_ = false;

private:
    uint64_t makeKey(int from, int to) const;
};

}  // namespace graph

#include "../src/GraphBase.tpp"
