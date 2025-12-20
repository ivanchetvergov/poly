#include "domain/shape_factory.h"
#include <stdexcept>

namespace domain {

std::unique_ptr<Shape> ShapeFactory::create(const ShapeData& data) {
    switch (data.type) {
        case ShapeType::Ellipse:
            return std::make_unique<Ellipse>(data);
        case ShapeType::Rectangle:
            return std::make_unique<Rectangle>(data);
        case ShapeType::Polygon:
            return std::make_unique<Polygon>(data);
    }
    throw std::runtime_error("Unknown shape type");
}

ShapeData ShapeFactory::createDefaultShapeData(ShapeType type, const QPointF& position) {
    ShapeData data;
    data.type = type;
    data.position = position;
    data.width = 80.0;
    data.height = 80.0;
    data.isVisible = true;
    
    if (type == ShapeType::Polygon) {
        data.polygonPoints = {
            QPointF(0, -40),
            QPointF(-40, 40),
            QPointF(40, 40)
        };
    }
    
    return data;
}

} // namespace domain