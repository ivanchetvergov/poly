#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"

namespace graph {

template<typename VertexT, typename EdgeT>
bool GraphBase<VertexT, EdgeT>::addVertex(int id) {
    if (m_vertices.find(id) != m_vertices.end()) {
        return false;
    }
    m_vertices[id] = std::make_unique<VertexT>(id);
    return true;
}

template<typename VertexT, typename EdgeT>
bool GraphBase<VertexT, EdgeT>::addEdge(int from, int to, const EdgeT& edge) {
    if (!hasVertex(from)) addVertex(from);
    if (!hasVertex(to)) addVertex(to);

    auto key = makeKey(from, to);
    if (m_edges.find(key) != m_edges.end()) {
        return false;
    }

    m_vertices[from]->addNeighbor(to);
    m_edges[key] = edge;
    return true;
}

template<typename VertexT, typename EdgeT>
size_t GraphBase<VertexT, EdgeT>::vertexCount() const noexcept {
    return m_vertices.size();
}

template<typename VertexT, typename EdgeT>
size_t GraphBase<VertexT, EdgeT>::edgeCount() const noexcept {
    return m_edges.size();
}

template<typename VertexT, typename EdgeT>
bool GraphBase<VertexT, EdgeT>::hasVertex(int id) const noexcept {
    return m_vertices.find(id) != m_vertices.end();
}

template<typename VertexT, typename EdgeT>
bool GraphBase<VertexT, EdgeT>::hasEdge(int from, int to) const {
    return m_edges.find(makeKey(from, to)) != m_edges.end();
}

template<typename VertexT, typename EdgeT>
std::optional<const VertexT*> GraphBase<VertexT, EdgeT>::getVertex(int id) const {
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) return std::nullopt;
    return it->second.get();
}

template<typename VertexT, typename EdgeT>
std::optional<EdgeT> GraphBase<VertexT, EdgeT>::getEdge(int from, int to) const {
    auto it = m_edges.find(makeKey(from, to));
    if (it == m_edges.end()) return std::nullopt;
    return it->second;
}

template<typename VertexT, typename EdgeT>
EdgeT* GraphBase<VertexT, EdgeT>::getEdgeMutable(int from, int to) {
    auto it = m_edges.find(makeKey(from, to));
    if (it == m_edges.end()) return nullptr;
    return &(it->second);
}

template<typename VertexT, typename EdgeT>
std::vector<int> GraphBase<VertexT, EdgeT>::vertexIds() const {
    std::vector<int> ids;
    ids.reserve(m_vertices.size());
    for (const auto& [id, _] : m_vertices) {
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

template<typename VertexT, typename EdgeT>
std::vector<int> GraphBase<VertexT, EdgeT>::neighbors(int id) const {
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) return {};
    return it->second->neighbors();
}

template<typename VertexT, typename EdgeT>
int GraphBase<VertexT, EdgeT>::degree(int v) const {
    auto it = m_vertices.find(v);
    if (it == m_vertices.end()) return 0;
    return static_cast<int>(it->second->neighbors().size());
}

template<typename VertexT, typename EdgeT>
std::vector<EdgeT> GraphBase<VertexT, EdgeT>::edges() const {
    std::vector<EdgeT> result;
    result.reserve(m_edges.size());
    for (const auto& [_, edge] : m_edges) {
        result.push_back(edge);
    }
    return result;
}

template<typename VertexT, typename EdgeT>
long long GraphBase<VertexT, EdgeT>::makeKey(int from, int to) const {
    if (!isDirected_ && from > to) {
        std::swap(from, to);
    }
    return (static_cast<long long>(from) << 32) | static_cast<unsigned int>(to);
}

} // namespace graph

