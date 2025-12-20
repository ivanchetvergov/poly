#pragma once

#include <QSqlDatabase>
#include <QString>
#include <optional>

namespace infrastructure {

/*
 * @brief Менеджер базы данных SQLite.
 *
 * Отвечает за инициализацию подключения к БД, создание таблиц
 * и управление жизненным циклом объекта QSqlDatabase.
 */
class DatabaseManager {
public:
    explicit DatabaseManager(const QString& dbPath = "shapes.db");
    ~DatabaseManager();

    // * --- Управление семантикой ---
    // ! Запрет копирования, т.к. QSqlDatabase не может быть скопирована
    DatabaseManager(const DatabaseManager&) = delete;            
    DatabaseManager& operator=(const DatabaseManager&) = delete; 

    // * --- Базовые методы для работы БД --- 
    [[nodiscard]] bool initialize();                    // скрипт инициализации таблицы
    [[nodiscard]] bool isConnected() const noexcept;    // проверка что бд открыта
    [[nodiscard]] QSqlDatabase& database() noexcept { return m_db; }
    [[nodiscard]] const QString& lastError() const noexcept { return m_lastError; }

private:
    bool createTables();    // скрипт создания таблиц
    
    QSqlDatabase m_db;      // объект MySQL базы
    QString m_dbPath;       // путь до нее
    QString m_lastError;    // лог ошибки
};

} // namespace infrastructure