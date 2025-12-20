#pragma once

#include "shape_types.h"
#include <QGraphicsObject>       // Используем QGraphicsObject для будущей поддержки сигналов
#include <QGraphicsEllipseItem>  // Требуется для Ellipse
#include <QGraphicsRectItem>     // Требуется для Rectangle
#include <QGraphicsPolygonItem>  // Требуется для Polygon
#include <QPainter>
#include <memory>
#include <QGraphicsLineItem>

namespace domain {

/*
 * @brief Базовый абстрактный класс для всех геометрических фигур.
 *
 * Теперь наследуется от QGraphicsObject для интеграции с Qt Framework.
 * Позволяет использовать сигналы/слоты.
 */
class Shape : public QGraphicsObject { // !!! QGraphicsObject !!!
    Q_OBJECT 
public:
    explicit Shape(const ShapeData& data);
    virtual ~Shape() = default;

    // * --- (Disable) ---
    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;
    Shape(Shape&&) = delete;
    Shape& operator=(Shape&&) = delete;

    // Геттеры
    [[nodiscard]] int id() const noexcept { return m_data.id; }
    [[nodiscard]] ShapeType shapeType() const noexcept { return m_data.type; }    
    [[nodiscard]] QPointF position() const noexcept { return pos(); }
    [[nodiscard]] bool isVisible() const noexcept { return m_data.isVisible; }
    [[nodiscard]] const std::vector<int>& connectedShapes() const noexcept { return m_data.connectedShapeIds; }
    [[nodiscard]] const ShapeData& data() const noexcept { return m_data; }

    // Сеттеры и управление связями
    void setPosition(const QPointF& pos) { setPos(pos); }
    void setShapeVisible(bool visible);
    void addConnection(int shapeId);
    void removeConnection(int shapeId);

    // QGraphicsItem интерфейс - делегируем дочерним классам
    QRectF boundingRect() const override = 0;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
               QWidget* widget = nullptr) override = 0;

    // Обработка изменений
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    // виртуальный метод для уведомления об изменении позиции
    virtual void notifyConnectionsOfPositionChange();
    // Обновить внутреннее состояние фигуры из ShapeData (применяется при внешнем обновлении)
    virtual void applyData(const ShapeData& data);

signals:
    void positionChanged(int shapeId, const QPointF& newPos);

protected:
    ShapeData m_data;
    QGraphicsItem* m_graphicsDelegate = nullptr;
    // вспомогательный метод для подсветки при наведении/выделении
    void drawSelectionHighlight(QPainter* painter) const;

    // Виртуальные методы для получения Pen и Brush, чтобы потомки могли их использовать
    virtual QPen shapePen() const { return QPen(Qt::black, 2); }
    virtual QBrush shapeBrush() const = 0; 
};

/*
 * @brief Реализация фигуры: Эллипс. Делегирует отрисовку QGraphicsEllipseItem.
 */
class Ellipse : public Shape {
public:
    explicit Ellipse(const ShapeData& data);
    
    // Мы явно переопределяем их для делегирования
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
               QWidget* widget = nullptr) override;
    
    enum { Type = UserType + 2 };
    int type() const override { return Type; }
protected:
    QBrush shapeBrush() const override;
    void applyData(const ShapeData& data) override;
};

/*
 * @brief Реализация фигуры: Прямоугольник. Делегирует отрисовку QGraphicsRectItem.
 */
class Rectangle : public Shape {
public:
    explicit Rectangle(const ShapeData& data);
    
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
               QWidget* widget = nullptr) override;
    
    enum { Type = UserType + 3 };
    int type() const override { return Type; }
protected:
    QBrush shapeBrush() const override;
    void applyData(const ShapeData& data) override;
};

/*
 * @brief Реализация фигуры: Многоугольник. Делегирует отрисовку QGraphicsPolygonItem.
 */
class Polygon : public Shape {
public:
    explicit Polygon(const ShapeData& data);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
               QWidget* widget = nullptr) override;
    
    enum { Type = UserType + 4 };
    int type() const override { return Type; }

private:
    QPolygonF getPolygon() const; // вспомогательный метод для получения полигона
protected:
    QBrush shapeBrush() const override;
    void applyData(const ShapeData& data) override;
};

/*
 * @brief Реализация фигуры: Соединение. Делегирует отрисовку QGraphicsPolygonItem.
 */
class ConnectionItem : public QGraphicsLineItem {
public:
    ConnectionItem(int fromId, int toId, Shape* fromShape, Shape* toShape,
                   QGraphicsItem* parent = nullptr);
    
    [[nodiscard]] int fromShapeId() const noexcept { return m_fromId; }
    [[nodiscard]] int toShapeId() const noexcept { return m_toId; }
    
    void updatePosition();
    
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;
    
    enum { Type = UserType + 10 };
    int type() const override { return Type; }

private:
    int m_fromId;
    int m_toId;
    Shape* m_fromShape; 
    Shape* m_toShape;   
    
    QPointF getEdgePoint(const QRectF& bounds, const QLineF& line, bool isStart) const;
};

} // namespace domain