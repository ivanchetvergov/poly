#include "Graph.h"

namespace graph {

template <VertexType VertexT, EdgeType EdgeT>
bool GraphBase<VertexT, EdgeT>::addVertex(int id) {
    if (m_vertices_.find(id) == m_vertices_.end()) {
        m_vertices_[id] = std::make_unique<VertexT>(id);
        return true;
    }
    return false;  // NOLINT(readability-simplify-boolean-expr)
}

template <VertexType VertexT, EdgeType EdgeT>
bool GraphBase<VertexT, EdgeT>::addEdge(int from, int to, EdgeT const& edge) {
    if (!hasVertex(from))
        addVertex(from);
    if (!hasVertex(to))
        addVertex(to);

    auto key = makeKey(from, to);
    if (m_edges_.find(key) == m_edges_.end()) {
        m_vertices_[from]->addNeighbor(to);
        m_edges_[key] = edge;
        return true;
    }
    return false;  // NOLINT(readability-simplify-boolean-expr)
}

template <VertexType VertexT, EdgeType EdgeT>
size_t GraphBase<VertexT, EdgeT>::vertexCount() const noexcept {
    return m_vertices_.size();
}

template <VertexType VertexT, EdgeType EdgeT>
size_t GraphBase<VertexT, EdgeT>::edgeCount() const noexcept {
    return m_edges_.size();
}

template <VertexType VertexT, EdgeType EdgeT>
bool GraphBase<VertexT, EdgeT>::hasVertex(int id) const noexcept {
    return m_vertices_.find(id) != m_vertices_.end();
}

template <VertexType VertexT, EdgeType EdgeT>
bool GraphBase<VertexT, EdgeT>::hasEdge(int from, int to) const {
    return m_edges_.find(makeKey(from, to)) != m_edges_.end();
}

template <VertexType VertexT, EdgeType EdgeT>
std::optional<VertexT const*> GraphBase<VertexT, EdgeT>::getVertex(int id) const {
    auto it = m_vertices_.find(id);
    if (it == m_vertices_.end())
        return std::nullopt;
    return it->second.get();
}

template <VertexType VertexT, EdgeType EdgeT>
std::optional<EdgeT> GraphBase<VertexT, EdgeT>::getEdge(int from, int to) const {
    auto it = m_edges_.find(makeKey(from, to));
    if (it == m_edges_.end())
        return std::nullopt;
    return it->second;
}

template <VertexType VertexT, EdgeType EdgeT>
EdgeT* GraphBase<VertexT, EdgeT>::getEdgeMutable(int from, int to) {
    auto it = m_edges_.find(makeKey(from, to));
    if (it == m_edges_.end())
        return nullptr;
    return &(it->second);
}

template <VertexType VertexT, EdgeType EdgeT>
std::vector<int> GraphBase<VertexT, EdgeT>::vertexIds() const {
    std::vector<int> ids;
    ids.reserve(m_vertices_.size());
    for (auto const& [id, _] : m_vertices_) {
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

template <VertexType VertexT, EdgeType EdgeT>
std::vector<int> GraphBase<VertexT, EdgeT>::neighbors(int id) const {
    auto it = m_vertices_.find(id);
    if (it == m_vertices_.end())
        return {};
    return it->second->neighbors();
}

template <VertexType VertexT, EdgeType EdgeT>
int GraphBase<VertexT, EdgeT>::degree(int v) const {
    auto it = m_vertices_.find(v);
    if (it == m_vertices_.end())
        return 0;
    return static_cast<int>(it->second->neighbors().size());
}

template <VertexType VertexT, EdgeType EdgeT>
std::vector<EdgeT> GraphBase<VertexT, EdgeT>::edges() const {
    std::vector<EdgeT> result;
    result.reserve(m_edges_.size());
    for (auto const& [_, edge] : m_edges_) {
        result.push_back(edge);
    }
    return result;
}

template <VertexType VertexT, EdgeType EdgeT>
uint64_t GraphBase<VertexT, EdgeT>::makeKey(int from, int to) const {
    if (!is_directed_ && from > to) {
        std::swap(from, to);
    }
    return (static_cast<uint64_t>(from) << 32) | static_cast<uint32_t>(to);
}

}  // namespace graph
