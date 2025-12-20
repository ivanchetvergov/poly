#pragma once

#include "domain/shape_types.h"
#include "database_manager.h"
#include <vector>
#include <optional>
#include <map>
#include <boost/signals2.hpp>

namespace infrastructure {

/*
 * @brief Репозиторий для управления сущностями фигур в базе данных.
 *
 * Класс ShapeRepository инкапсулирует логику доступа к данным,
 * скрывая детали работы с SQL (QSqlDatabase) от вышестоящих слоев (Presentation).
 */
class ShapeRepository {
public:
    explicit ShapeRepository(DatabaseManager& dbManager);

    // * CRUD методы
    [[nodiscard]] std::optional<int> save(const domain::ShapeData& data);
    [[nodiscard]] bool update(const domain::ShapeData& data);
    [[nodiscard]] bool remove(int shapeId);
    [[nodiscard]] std::optional<domain::ShapeData> findById(int id) const;
    [[nodiscard]] std::vector<domain::ShapeData> findAll() const;
    [[nodiscard]] std::vector<domain::ShapeData> findByType(domain::ShapeType type) const;
    [[nodiscard]] std::map<domain::ShapeType, int> countByType() const;
    [[nodiscard]] bool updatePosition(int shapeId, const QPointF& position);

    // * Методы для управления связями 
    bool addConnection(int shapeId, int connectedShapeId);
    bool removeConnection(int shapeId, int connectedShapeId);
    bool removeAllConnectionsForShape(int shapeId);
    
    //*  --- Signals (Boost.Signals2) ---    
    // (замена QT сигналов не требует MOC) (consumer - gr_view)
    //* void - ничего не возращает, принимает int
    boost::signals2::signal<void(int)> shapeAdded;
    boost::signals2::signal<void(int)> shapeRemoved;
    boost::signals2::signal<void(int)> shapeUpdated;
    // boost::signals2::signal<void(int)> shapeUpdated;
    // boost::signals2::signal<void(int)> connectionsChanged;

private:
    // * Приватные методы
    std::vector<int> loadConnections(int shapeId) const;                    // загружает ID связанныъ фигур
    bool saveConnections(int shapeId, const std::vector<int>& connections); // сохраняе связи фигуры
    // * Работа с БД
    QString serializePolygonPoints(const std::vector<QPointF>& points) const;
    std::vector<QPointF> deserializePolygonPoints(const QString& data) const;

    DatabaseManager& m_dbManager; // ссылка на менеджер БД 
};

} // namespace infrastructure