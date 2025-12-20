#include "infrastructure/database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace infrastructure {

DatabaseManager::DatabaseManager(const QString& dbPath) 
    : m_dbPath(dbPath) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initialize() {
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qWarning() << "Failed to open database:" << m_lastError;
        return false;
    }

    return createTables();
}

bool DatabaseManager::isConnected() const noexcept {
    return m_db.isOpen();
}

bool DatabaseManager::createTables() {
    // инициализируем объект QSqlQuery, 
    // привязывая его к открытому соединению (m_db)
    QSqlQuery query(m_db);

    // запрос на создание таблицы фигур (Shapes)
    const QString createShapesTable = R"(
        CREATE TABLE IF NOT EXISTS shapes (
            id INTEGER PRIMARY KEY AUTOINCREMENT, -- Уникальный идентификатор фигуры (ShapeData::id)
            type TEXT NOT NULL,                   -- Тип фигуры ('Ellipse', 'Rectangle', 'Polygon')
            position_x REAL NOT NULL,             -- X-координата центра фигуры (ShapeData::position.x)
            position_y REAL NOT NULL,             -- Y-координата центра фигуры (ShapeData::position.y)
            width REAL NOT NULL,                  -- Ширина фигуры (ShapeData::width)
            height REAL NOT NULL,                 -- Высота фигуры (ShapeData::height)
            polygon_points TEXT,                  -- Сериализованный список точек для полигона (ShapeData::polygonPoints). NULL для Ellipse/Rectangle.
            is_visible INTEGER DEFAULT 1          -- Флаг видимости (ShapeData::isVisible). 1 - true, 0 - false.
        )
    )";
    
    if (!query.exec(createShapesTable)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create shapes table:" << m_lastError;
        return false;
    }
    
    // таблица связей между фигурами (Shape Connections)
    const QString createConnectionsTable = R"(
        CREATE TABLE IF NOT EXISTS shape_connections (
            id INTEGER PRIMARY KEY AUTOINCREMENT,              -- Уникальный ID связи
            shape_id INTEGER NOT NULL,                         -- ID фигуры-источника связи
            connected_shape_id INTEGER NOT NULL,               -- ID фигуры-назначения связи (ShapeData::connectedShapeIds)
            FOREIGN KEY (shape_id) REFERENCES shapes(id) ON DELETE CASCADE,           -- При удалении shape_id связь также удаляется
            FOREIGN KEY (connected_shape_id) REFERENCES shapes(id) ON DELETE CASCADE, -- При удалении connected_shape_id связь также удаляется
            UNIQUE(shape_id, connected_shape_id)               -- Гарантирует, что связь между A и B существует только один раз
        )
    )";
    
    if (!query.exec(createConnectionsTable)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create connections table:" << m_lastError;
        return false;
    }
    
    // включаем поддержку внешних ключей
    // (по умолчанию, SQLite игнорирует ограничения внешних ключей)
    query.exec("PRAGMA foreign_keys = ON");
    
    return true;
}

} // namespace infrastructure