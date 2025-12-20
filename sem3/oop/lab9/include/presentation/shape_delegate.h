#pragma once

#include "domain/shape_types.h"
#include <QStyledItemDelegate>

namespace presentation {
/*
 * @brief Делегат для кастомизации отрисовки элементов в QTableView.
 *
 * Отвечает за специализированную отрисовку данных (например, типа фигуры)
 * в табличном представлении, а также за корректный расчет размеров ячеек.
 */
class ShapeDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ShapeDelegate(QObject* parent = nullptr);
    
    // главная функция отрисовки ячейки
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    // определяет оптимальный размер ячейки
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const override;

private:
    // отрисовка типа фигурки 
    void drawShapeIcon(QPainter* painter, const QRectF& rect, 
                      domain::ShapeType type) const;
    // возращает кол-во фигурок в колонку
    [[nodiscard]] int getIconCount(int shapeCount) const noexcept;
};

} // namespace presentation