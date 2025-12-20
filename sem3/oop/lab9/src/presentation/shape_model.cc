#include "presentation/shape_model.h"
#include <algorithm>

namespace presentation {

ShapeModel::ShapeModel(std::shared_ptr<infrastructure::ShapeRepository> repository,
                       QObject* parent)
    : QAbstractTableModel(parent)
    , m_repository(repository) {
    
    // Подключаем сигналы репозитория
    m_repository->shapeAdded.connect([this](int id) { onShapeAdded(id); });
    // ! решение проблемы с SIGIT. автоматический сигнал вызывает вложенный вызов
    // m_repository->shapeRemoved.connect([this](int id) { onShapeRemoved(id); });
    
    refresh();
}

int ShapeModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_filteredShapes.size());
}

int ShapeModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return ColumnCount;
}

QVariant ShapeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(m_filteredShapes.size())) {
        return QVariant();
    }

    const auto& shape = m_filteredShapes[index.row()];
    
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case IdColumn:
                return shape.id;
            case TypeColumn:
                return domain::shapeTypeToString(shape.type);
            case CountColumn:
                return getCountForType(shape.type);
            case ConnectionsColumn:
                return getConnectionsString(shape.connectedShapeIds);
        }
    } else if (role == Qt::UserRole) {
        // для делегата: количество фигур данного типа
        return getCountForType(shape.type);
    } else if (role == Qt::UserRole + 1) {
        // тип фигуры
        return static_cast<int>(shape.type);
    }
    
    return QVariant();
}

QVariant ShapeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }
    
    switch (section) {
        case IdColumn: return "ID";
        case TypeColumn: return "Тип";
        case CountColumn: return "Количество";
        case ConnectionsColumn: return "Связи";
        default: return QVariant();
    }
}

Qt::ItemFlags ShapeModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool ShapeModel::removeRow(int row, const QModelIndex& parent) {
    if (parent.isValid() || row < 0 || row >= static_cast<int>(m_filteredShapes.size())) {
        return false;
    }
    
    const int shapeId = m_filteredShapes[row].id;

    // 1. сначала удаляем из БД, чтобы убедиться в успехе
    bool success = m_repository->remove(shapeId);
    
    if (success) {
        // ! 2. ВРУЧНУЮ ПОДАЕМ СИГНАЛ (вызываем полную перезагрузку модели)
        onShapeRemoved(shapeId);
    }
    return success;
}


void ShapeModel::refresh() {
    beginResetModel();
    m_shapes = m_repository->findAll();
    m_typeCounts = m_repository->countByType();
    updateFilteredShapes();
    endResetModel();
    emit shapeVisibilityChanged();
}

void ShapeModel::setTypeFilter(domain::ShapeType type, bool visible) {
    if (visible) {
        m_hiddenTypes.remove(type);
    } else {
        m_hiddenTypes.insert(type);
    }
    
    beginResetModel();
    updateFilteredShapes();
    endResetModel();
    emit shapeVisibilityChanged();
}

void ShapeModel::setTypeFilters(bool ellipseVisible, bool rectangleVisible, bool polygonVisible) {
    // Update all three filters in one shot and emit a single visibility change
    if (ellipseVisible) m_hiddenTypes.remove(domain::ShapeType::Ellipse);
    else m_hiddenTypes.insert(domain::ShapeType::Ellipse);

    if (rectangleVisible) m_hiddenTypes.remove(domain::ShapeType::Rectangle);
    else m_hiddenTypes.insert(domain::ShapeType::Rectangle);

    if (polygonVisible) m_hiddenTypes.remove(domain::ShapeType::Polygon);
    else m_hiddenTypes.insert(domain::ShapeType::Polygon);

    beginResetModel();
    updateFilteredShapes();
    endResetModel();
    emit shapeVisibilityChanged();
}

void ShapeModel::clearFilters() {
    m_hiddenTypes.clear();
    beginResetModel();
    updateFilteredShapes();
    endResetModel();
    emit shapeVisibilityChanged();
}

domain::ShapeData ShapeModel::getShapeData(int row) const {
    if (row >= 0 && row < static_cast<int>(m_filteredShapes.size())) {
        return m_filteredShapes[row];
    }
    return domain::ShapeData{};
}

void ShapeModel::onShapeAdded([[maybe_unused]] int id) {
    beginResetModel();
    m_shapes = m_repository->findAll();
    m_typeCounts = m_repository->countByType();
    updateFilteredShapes();
    endResetModel();
    emit shapeVisibilityChanged();
}

void ShapeModel::onShapeRemoved([[maybe_unused]] int id) {
    beginResetModel();
    m_shapes = m_repository->findAll();
    m_typeCounts = m_repository->countByType();
    updateFilteredShapes();
    endResetModel();
    emit shapeVisibilityChanged();
}

void ShapeModel::updateFilteredShapes() {
    m_filteredShapes.clear();
    
    for (const auto& shape : m_shapes) {
        if (!m_hiddenTypes.contains(shape.type) && shape.isVisible) {
            m_filteredShapes.push_back(shape);
        }
    }
}

int ShapeModel::getCountForType(domain::ShapeType type) const {
    auto it = m_typeCounts.find(type);
    return it != m_typeCounts.end() ? it->second : 0;
}

QString ShapeModel::getConnectionsString(const std::vector<int>& connections) const {
    if (connections.empty()) {
        return "нет";
    }
    
    QStringList idList;
    for (int id : connections) {
        idList << QString::number(id);
    }
    return idList.join(", ");
}

} // namespace presentation