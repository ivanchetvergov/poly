#include "presentation/shape_delegate.h"
#include "presentation/shape_model.h"
#include <QPainter>
#include <QPen>
#include <QBrush>

namespace presentation {

ShapeDelegate::ShapeDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

void ShapeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                         const QModelIndex& index) const {
    if (index.column() != ShapeModel::CountColumn) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }
    
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    
    // получаем данные из модели
    int shapeCount = index.data(Qt::UserRole).toInt();
    int shapeTypeInt = index.data(Qt::UserRole + 1).toInt();
    auto shapeType = static_cast<domain::ShapeType>(shapeTypeInt);
    
    // определяем количество иконок для отрисовки
    int iconCount = getIconCount(shapeCount);
    
    // рисуем фон ячейки
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    } else {
        painter->fillRect(option.rect, option.palette.base());
    }
    
    // параметры для отрисовки иконок
    const int iconSize = 24;
    const int spacing = 4;
    const int totalWidth = iconCount * iconSize + (iconCount - 1) * spacing;
    const int startX = option.rect.center().x() - totalWidth / 2;
    const int y = option.rect.center().y();
    
    // рисуем иконки
    for (int i = 0; i < iconCount; ++i) {
        QRectF iconRect(startX + i * (iconSize + spacing), 
                        y - iconSize / 2,
                        iconSize, 
                        iconSize);
        drawShapeIcon(painter, iconRect, shapeType);
    }
    
    painter->restore();
}

QSize ShapeDelegate::sizeHint(const QStyleOptionViewItem& option,
                              const QModelIndex& index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    
    if (index.column() == ShapeModel::CountColumn) {
        size.setHeight(36);
    }
    
    return size;
}

void ShapeDelegate::drawShapeIcon(QPainter* painter, const QRectF& rect,
                                 domain::ShapeType type) const {
    QPen pen(Qt::black, 1.5);
    painter->setPen(pen);
    
    switch (type) {
        case domain::ShapeType::Ellipse: {
            painter->setBrush(QBrush(QColor(100, 149, 237, 180)));
            painter->drawEllipse(rect);
            break;
        }
        case domain::ShapeType::Rectangle: {
            painter->setBrush(QBrush(QColor(144, 238, 144, 180)));
            painter->drawRect(rect);
            break;
        }
        case domain::ShapeType::Polygon: {
            painter->setBrush(QBrush(QColor(255, 182, 193, 180)));
            QPolygonF triangle;
            triangle << QPointF(rect.center().x(), rect.top())
                    << QPointF(rect.left(), rect.bottom())
                    << QPointF(rect.right(), rect.bottom());
            painter->drawPolygon(triangle);
            break;
        }
    }
}

int ShapeDelegate::getIconCount(int shapeCount) const noexcept {
    if (shapeCount >= 1 && shapeCount <= 3) {
        return 1;
    } else if (shapeCount >= 4 && shapeCount <= 10) {
        return 2;
    } else if (shapeCount > 10) {
        return 3;
    }
    return 0;
}

} // namespace presentation