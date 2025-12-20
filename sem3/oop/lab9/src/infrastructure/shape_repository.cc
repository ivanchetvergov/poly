#include "infrastructure/shape_repository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

namespace infrastructure {

ShapeRepository::ShapeRepository(DatabaseManager& dbManager) 
    : m_dbManager(dbManager) {}

// * CREATE - создание новой фигуры в БД
std::optional<int> ShapeRepository::save(const domain::ShapeData& data) {
    QSqlQuery query(m_dbManager.database());
    
    // * Создаём подготовленный SQL-запрос 
    query.prepare(R"(
        INSERT INTO shapes (type, position_x, position_y, width, height, polygon_points, is_visible)
        VALUES (:type, :pos_x, :pos_y, :width, :height, :polygon_points, :visible)
    )");
    
    // * Биндим значения к плейсхолдерам
    query.bindValue(":type", domain::shapeTypeToString(data.type));
    query.bindValue(":pos_x", data.position.x());
    query.bindValue(":pos_y", data.position.y());
    query.bindValue(":width", data.width);
    query.bindValue(":height", data.height);
    query.bindValue(":polygon_points", serializePolygonPoints(data.polygonPoints));
    query.bindValue(":visible", data.isVisible ? 1 : 0);
    
    if (!query.exec()) {
        qWarning() << "Failed to save shape:" << query.lastError().text();
        return std::nullopt;
    }
    
    // получаем ID только что созданной записи
    int newId = query.lastInsertId().toInt();
    
    // * Сохраняем связи с другими фигурами (если есть)
    if (!data.connectedShapeIds.empty()) {
        saveConnections(newId, data.connectedShapeIds);
    }
    
    // * Эмитим сигнал о добавлении фигуры (Boost.Signals2)
    qDebug() << "ShapeRepository::save - newId" << newId;
    shapeAdded(newId);

    return newId; // возвращаем новый ID
}

// * UPDATE - обновление всех полей фигуры в БД
bool ShapeRepository::update(const domain::ShapeData& data) {
    qDebug() << "ShapeRepository::update - executing update for id" << data.id << "pos" << data.position;
    if (data.id < 0) {
        return false;
    }
    
    QSqlQuery query(m_dbManager.database());
    
    // * Подготовленный запрос для обновления ВСЕХ полей фигуры
    query.prepare(R"(
        UPDATE shapes 
        SET type = :type, position_x = :pos_x, position_y = :pos_y,
            width = :width, height = :height, polygon_points = :polygon_points,
            is_visible = :visible
        WHERE id = :id
    )");
    
    // биндим все поля
    query.bindValue(":id", data.id);
    query.bindValue(":type", domain::shapeTypeToString(data.type));
    query.bindValue(":pos_x", data.position.x());
    query.bindValue(":pos_y", data.position.y());
    query.bindValue(":width", data.width);
    query.bindValue(":height", data.height);
    query.bindValue(":polygon_points", serializePolygonPoints(data.polygonPoints));
    query.bindValue(":visible", data.isVisible ? 1 : 0);
    
    if (!query.exec()) {
        qWarning() << "Failed to update shape:" << query.lastError().text();
        return false;
    }

    // * Эмитим сигнал об обновлении фигуры
    qDebug() << "ShapeRepository::update - emit shapeUpdated for id" << data.id;
    shapeUpdated(data.id);

    return true;
}

// * DELETE - удаление фигуры из БД
bool ShapeRepository::remove(int shapeId) {
    QSqlQuery checkQuery(m_dbManager.database());
    
    // * Сначала проверяем, какие фигуры были связаны с удаляемой
    checkQuery.prepare("SELECT shape_id FROM shape_connections WHERE connected_shape_id = :id");
    checkQuery.bindValue(":id", shapeId);
    
    // собираем ID затронутых фигур
    std::vector<int> affectedIds;
    if (checkQuery.exec()) {
        while (checkQuery.next()) {
            affectedIds.push_back(checkQuery.value(0).toInt());
        }
    }
    
    // * Удаляем все связи этой фигуры (CASCADE должен сработать автоматически)
    removeAllConnectionsForShape(shapeId);
    
    // * Удаляем саму фигуру
    QSqlQuery query(m_dbManager.database());
    query.prepare("DELETE FROM shapes WHERE id = :id");
    query.bindValue(":id", shapeId);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove shape:" << query.lastError().text();
        return false;
    }
    
    // * Эмитим сигнал об удалении фигуры
    qDebug() << "ShapeRepository::remove - removed id" << shapeId;
    shapeRemoved(shapeId);

    // TODO: можно эмитить shapeUpdated для затронутых фигур (affectedIds)
    return true;
}

// * Обновление ТОЛЬКО позиции фигуры в БД
bool ShapeRepository::updatePosition(int shapeId, const QPointF& position) {
    if (shapeId < 0) return false;
    
    QSqlQuery query(m_dbManager.database());
    
    // * Обновляем только координаты, не трогая остальные поля
    query.prepare(R"(
        UPDATE shapes 
        SET position_x = :pos_x, position_y = :pos_y
        WHERE id = :id
    )");
    
    query.bindValue(":id", shapeId);
    query.bindValue(":pos_x", position.x());
    query.bindValue(":pos_y", position.y());
    
    qDebug() << "ShapeRepository::updatePosition - updating id" << shapeId << "to" << position;
    if (!query.exec()) {
        qWarning() << "Failed to update shape position:" << query.lastError().text();
        return false;
    }

    // * Эмитим сигнал об обновлении фигуры
    qDebug() << "ShapeRepository::updatePosition - emit shapeUpdated for id" << shapeId;
    shapeUpdated(shapeId);

    return true;
}

std::optional<domain::ShapeData> ShapeRepository::findById(int id) const {
    QSqlQuery query(m_dbManager.database());
    query.prepare("SELECT * FROM shapes WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    domain::ShapeData data;
    data.id = query.value("id").toInt();
    
    auto typeOpt = domain::stringToShapeType(query.value("type").toString());
    if (!typeOpt) {
        return std::nullopt;
    }
    data.type = *typeOpt;
    
    data.position = QPointF(query.value("position_x").toDouble(),
                            query.value("position_y").toDouble());
    data.width = query.value("width").toDouble();
    data.height = query.value("height").toDouble();
    data.polygonPoints = deserializePolygonPoints(query.value("polygon_points").toString());
    data.isVisible = query.value("is_visible").toInt() != 0;
    data.connectedShapeIds = loadConnections(data.id);
    
    return data;
}

std::vector<domain::ShapeData> ShapeRepository::findAll() const {
    std::vector<domain::ShapeData> shapes;

    QSqlQuery query(m_dbManager.database());
    if (!query.exec("SELECT * FROM shapes")) {
        qWarning() << "Failed to load shapes:" << query.lastError().text();
        return shapes;
    }

    while (query.next()) {
        auto shape = findById(query.value("id").toInt());
        if (shape) {
            shapes.push_back(*shape);
        }
    }

    return shapes;
}

std::vector<domain::ShapeData> ShapeRepository::findByType(domain::ShapeType type) const {
    std::vector<domain::ShapeData> shapes;

    QSqlQuery query(m_dbManager.database());
    query.prepare("SELECT * FROM shapes WHERE type = :type");
    query.bindValue(":type", domain::shapeTypeToString(type));

    if (!query.exec()) {
        qWarning() << "Failed to load shapes by type:" << query.lastError().text();
        return shapes;
    }

    while (query.next()) {
        auto shape = findById(query.value("id").toInt());
        if (shape) {
            shapes.push_back(*shape);
        }
    }

    return shapes;
}

std::map<domain::ShapeType, int> ShapeRepository::countByType() const {
    std::map<domain::ShapeType, int> counts;
    
    QSqlQuery query(m_dbManager.database());
    if (!query.exec("SELECT type, COUNT(*) as count FROM shapes GROUP BY type")) {
        qWarning() << "Failed to count shapes:" << query.lastError().text();
        return counts;
    }
    
    while (query.next()) {
        auto typeOpt = domain::stringToShapeType(query.value("type").toString());
        if (typeOpt) {
            counts[*typeOpt] = query.value("count").toInt();
        }
    }
    
    return counts;
}

bool ShapeRepository::addConnection(int shapeId, int connectedShapeId) {
    QSqlQuery query(m_dbManager.database());
    query.prepare(R"(
        INSERT OR IGNORE INTO shape_connections (shape_id, connected_shape_id)
        VALUES (:shape_id, :connected_id)
    )");
    query.bindValue(":shape_id", shapeId);
    query.bindValue(":connected_id", connectedShapeId);
    
    return query.exec();
}

bool ShapeRepository::removeConnection(int shapeId, int connectedShapeId) {
    QSqlQuery query(m_dbManager.database());
    query.prepare(R"(
        DELETE FROM shape_connections 
        WHERE shape_id = :shape_id AND connected_shape_id = :connected_id
    )");
    query.bindValue(":shape_id", shapeId);
    query.bindValue(":connected_id", connectedShapeId);
    
    return query.exec();
}

bool ShapeRepository::removeAllConnectionsForShape(int shapeId) {
    QSqlQuery query(m_dbManager.database());
    query.prepare(R"(
        DELETE FROM shape_connections 
        WHERE shape_id = :id OR connected_shape_id = :id
    )");
    query.bindValue(":id", shapeId);
    
    return query.exec();
}

std::vector<int> ShapeRepository::loadConnections(int shapeId) const {
    std::vector<int> connections;
    
    QSqlQuery query(m_dbManager.database());
    query.prepare("SELECT connected_shape_id FROM shape_connections WHERE shape_id = :id");
    query.bindValue(":id", shapeId);
    
    if (query.exec()) {
        while (query.next()) {
            connections.push_back(query.value(0).toInt());
        }
    }
    
    return connections;
}

bool ShapeRepository::saveConnections(int shapeId, const std::vector<int>& connections) {
    for (int connectedId : connections) {
        if (!addConnection(shapeId, connectedId)) {
            return false;
        }
    }
    return true;
}

QString ShapeRepository::serializePolygonPoints(const std::vector<QPointF>& points) const {
    if (points.empty()) {
        return QString();
    }
    
    QJsonArray jsonArray;
    for (const auto& pt : points) {
        QJsonObject ptObj;
        ptObj["x"] = pt.x();
        ptObj["y"] = pt.y();
        jsonArray.append(ptObj);
    }
    // ! возвращаем JSON как string чтобы хранить в таблице
    return QJsonDocument(jsonArray).toJson(QJsonDocument::Compact);
}

std::vector<QPointF> ShapeRepository::deserializePolygonPoints(const QString& data) const {
    std::vector<QPointF> points;
    
    if (data.isEmpty()) {
        return points;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (!doc.isArray()) {
        return points;
    }
    
    QJsonArray jsonArray = doc.array();
    for (const auto& val : jsonArray) {
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            points.emplace_back(obj["x"].toDouble(), obj["y"].toDouble());
        }
    }
    
    return points;
}

} // namespace infrastructure