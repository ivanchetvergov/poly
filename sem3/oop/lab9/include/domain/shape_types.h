#pragma once

#include <QString>
#include <QPointF>
#include <optional>
#include <vector>
#include <memory>

namespace domain {

/*
 * @brief Перечисление типов геометрических фигур.
 */
enum class ShapeType {
    Ellipse,
    Rectangle,
    Polygon
};

/*
 * @brief Структура данных, хранящая состояние фигуры.
 */
struct ShapeData {
    int id{-1};
    ShapeType type{ShapeType::Rectangle};
    QPointF position{0.0, 0.0};
    double width{100.0};
    double height{100.0};
    std::vector<QPointF> polygonPoints;
    std::vector<int> connectedShapeIds;
    bool isVisible{true};

    // * --- правило пяти (The Rule of Five) ---
    ShapeData() = default; 
    ShapeData(const ShapeData& other) = default;
    ShapeData& operator=(const ShapeData& other) = default;
    ShapeData(ShapeData&& other) noexcept = default;
    ShapeData& operator=(ShapeData&& other) noexcept = default;
    
    ~ShapeData() = default;
};

// * --- методы для преобразования фигуры ---

inline QString shapeTypeToString(ShapeType type) noexcept { // Добавлено noexcept
    switch (type) {
        case ShapeType::Ellipse: return "Ellipse";
        case ShapeType::Rectangle: return "Rectangle";
        case ShapeType::Polygon: return "Polygon";
    }
    return "Unknown";
}

inline std::optional<ShapeType> stringToShapeType(const QString& str) noexcept { // Добавлено noexcept
    if (str == "Ellipse") return ShapeType::Ellipse;
    if (str == "Rectangle") return ShapeType::Rectangle;
    if (str == "Polygon") return ShapeType::Polygon;
    return std::nullopt;
}

} // namespace domain