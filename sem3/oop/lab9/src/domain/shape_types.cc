#include "domain/shape_types.h"

namespace domain {

// ShapeData& ShapeData::operator=(ShapeData&& other) noexcept {
//     if (this != &other) {
//         id = other.id;
//         type = other.type;
//         width = other.width;
//         height = other.height;
//         isVisible = other.isVisible;
        
//         // выполняем move-присваивание для нетривиальных членов. 
//         position = std::move(other.position);
//         polygonPoints = std::move(other.polygonPoints);
//         connectedShapeIds = std::move(other.connectedShapeIds);

//         // обнуляем исходник
//         other.id = -1;
//         other.width = 0;
//         other.height = 0;
//         other.isVisible = false;
//     }
//     return *this;
// }

// ShapeData::ShapeData(ShapeData&& other) noexcept
//     : id(other.id)
//     , type(other.type)
//     , position(std::move(other.position))
//     , width(other.width)
//     , height(other.height)
//     , polygonPoints(std::move(other.polygonPoints))
//     , connectedShapeIds(std::move(other.connectedShapeIds))
//     , isVisible(other.isVisible)
// {
//     other.id = -1;
//     other.width = 0;
//     other.height = 0;
//     other.isVisible = false;
// }



} // namespace domain