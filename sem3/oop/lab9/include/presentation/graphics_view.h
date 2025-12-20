#pragma once

#include "domain/shape.h"
#include "infrastructure/shape_repository.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <memory>
#include <unordered_map>

namespace presentation {
/*
 * @brief Пользовательское представление для отображения фигур (QGraphicsView).
 *
 * Теперь использует стандартные QGraphicsItem вместо ручной отрисовки.
 * Отвечает за визуализацию всех объектов-фигур, их связей и обработку
 * пользовательского взаимодействия в графической области. Подписывается
 * на сигналы репозитория для автоматического обновления.
 */
class GraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    explicit GraphicsView(std::shared_ptr<infrastructure::ShapeRepository> repository,
                         QWidget* parent = nullptr);

    void addShape(domain::Shape* shape);        // добавляет фигуру на сцену
    void refresh();                             // принудительно перезагружает все фигуры
    void clearShapes();                         // очищает все фигуры со сцены
    void updateShapeVisibility(int shapeId, bool visible); // меняет видимость фигуры

signals:
    void shapeSelected(int shapeId);                                    // сигнал при выборе фигуры
    void shapePositionChanged(int shapeId, const QPointF& newPosition); // сигнал при изменении позиции

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override; // рисует сетку
    void mousePressEvent(QMouseEvent* event) override;                   // обработка кликов
    void mouseReleaseEvent(QMouseEvent* event) override;                 // обработка отпускания мыши

private slots:
    void onShapeAdded(int id);          // обрабатывает сигнал о добавлении фигуры
    void onShapeRemoved(int id);        // обрабатывает сигнал об удалении фигуры
    void onShapeUpdated(int id);        // обрабатывает сигнал об обновлении фигуры
    void onShapeMoved();                // обрабатывает перемещение фигуры

private:
    void loadAllShapes();               // загружает все фигуры из репозитория
    void updateConnections();           // обновляет все линии связей между фигурами
    void createConnectionItems();       // создаёт графические элементы для связей
    
    // * UI / Core
    QGraphicsScene* m_scene;            // графическая сцена Qt
    std::shared_ptr<infrastructure::ShapeRepository> m_repository; // репозиторий БД
    
    // * Хранилища графических элементов
    // кэш всех фигур: ID -> указатель на Shape (сцена владеет объектами)
    std::unordered_map<int, domain::Shape*> m_shapes;
    
    // список всех линий связей (сцена владеет объектами)
    std::vector<domain::ConnectionItem*> m_connections;
    
    // * Состояние взаимодействия
    bool m_isMousePressed{false};       // флаг нажатия мыши
    QPointF m_lastMousePos;             // последняя позиция мыши
};

} // namespace presentation