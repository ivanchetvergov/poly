#pragma once

#include "shape.h"
#include <memory>

namespace domain {
/*
 * @brief Фабрика для создания объектов-фигур.
 *
 * Класс инкапсулирует логику создания:
 * 1. Полиморфных объектов-фигур (Shape) по заданным данным.
 * 2. Начальных структур ShapeData с дефолтными значениями.
 */
class ShapeFactory {
public:
    static std::unique_ptr<Shape> create(const ShapeData& data);
    
    static ShapeData createDefaultShapeData(ShapeType type, const QPointF& position = QPointF(0, 0));
};

} // namespace domain