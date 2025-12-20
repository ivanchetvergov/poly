#include "presentation/graphics_view.h"
#include "domain/shape_factory.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

namespace presentation {

GraphicsView::GraphicsView(std::shared_ptr<infrastructure::ShapeRepository> repository,
                           QWidget* parent)
    : QGraphicsView(parent)
    , m_repository(repository) {
    
    // * Создаём и настраиваем сцену для отрисовки фигур
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-500, -300, 1300, 500); // устанавливаем границы сцены
    setScene(m_scene);
    
    // * Настройки рендеринга для красивой отрисовки
    setRenderHint(QPainter::Antialiasing);                     // сглаживание краёв
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);  // полное обновление при изменениях
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // * Режим взаимодействия с мышью
    setDragMode(QGraphicsView::RubberBandDrag);                 // резиновое выделение области
    
    // * Подключаем сигналы репозитория (Boost.Signals2) для реакции на изменения в БД
    m_repository->shapeAdded.connect([this](int id) { onShapeAdded(id); });
    m_repository->shapeRemoved.connect([this](int id) { onShapeRemoved(id); });
    m_repository->shapeUpdated.connect([this](int id) { onShapeUpdated(id); });
    
    // * Подключаем сигнал сцены для обновления позиций линий связей
    connect(m_scene, &QGraphicsScene::changed, this, &GraphicsView::onShapeMoved);
    
    // загружаем все фигуры из БД при инициализации
    loadAllShapes();
}

void GraphicsView::addShape(domain::Shape* shape) {
    if (!shape) {
        return;
    }
    
    int id = shape->id();
    
    // * Добавляем фигуру на сцену
    m_scene->addItem(shape);
    m_shapes[id] = shape;  // сохраняем указатель в кэше для быстрого доступа по ID
    
    // обновляем линии связей после добавления новой фигуры
    updateConnections();
}

void GraphicsView::refresh() {
    // * Полное обновление сцены: удаляем все фигуры и загружаем заново из БД
    clearShapes();
    loadAllShapes();
}

void GraphicsView::clearShapes() {
    // * Удаляем все линии связей со сцены
    for (auto* connection : m_connections) {
        m_scene->removeItem(connection);
        delete connection;
    }
    m_connections.clear();
    
    // * Удаляем все фигуры со сцены
    for (auto& [id, shape] : m_shapes) {
        m_scene->removeItem(shape);
        delete shape;  // сцена больше не владеет, поэтому удаляем вручную
    }
    m_shapes.clear();
}

void GraphicsView::updateShapeVisibility(int shapeId, bool visible) {
    // * Обновляем видимость конкретной фигуры (для фильтров)
    auto it = m_shapes.find(shapeId);
    if (it != m_shapes.end() && it->second) {
        it->second->setShapeVisible(visible);
        
        // обновляем связи, т.к. видимость фигуры изменилась 
        updateConnections();
    }
}

void GraphicsView::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawBackground(painter, rect);
    
    // рисуем сетку для удобства позиционирования
    painter->setPen(QPen(QColor(220, 220, 220), 1));
    
    const int gridSize = 50;
    int left = static_cast<int>(rect.left()) - (static_cast<int>(rect.left()) % gridSize);
    int top = static_cast<int>(rect.top()) - (static_cast<int>(rect.top()) % gridSize);
    
    // вертикальные линии
    for (int x = left; x < rect.right(); x += gridSize) {
        painter->drawLine(x, static_cast<int>(rect.top()), 
                         x, static_cast<int>(rect.bottom()));
    }
    
    // горизонтальные линии
    for (int y = top; y < rect.bottom(); y += gridSize) {
        painter->drawLine(static_cast<int>(rect.left()), y, 
                         static_cast<int>(rect.right()), y);
    }
}

void GraphicsView::onShapeAdded(int id) {
    // * Обработчик добавления фигуры в БД (вызывается через Boost.Signals2)
    auto shapeData = m_repository->findById(id);
    if (shapeData) {
        // создаём фигуру через фабрику
        auto shapePtr = domain::ShapeFactory::create(*shapeData);
        
        // добавляем на сцену (передаём владение)
        addShape(shapePtr.release());
    }
}

void GraphicsView::onShapeRemoved(int id) {
    // * Обработчик удаления фигуры из БД (вызывается через Boost.Signals2)
    auto it = m_shapes.find(id);
    if (it != m_shapes.end()) {
        domain::Shape* shape = it->second;
        
        // удаляем со сцены
        m_scene->removeItem(shape);
        delete shape;
        
        // удаляем из кэша
        m_shapes.erase(it);
        
        // обновляем связи (удаляем линии, которые вели к этой фигуре)
        updateConnections();
    }

}

void GraphicsView::onShapeUpdated(int id) {
    // * Обработчик обновления фигуры в БД (вызывается через Boost.Signals2)
    auto shapeData = m_repository->findById(id);
    if (!shapeData) return;

    auto it = m_shapes.find(id);
    if (it != m_shapes.end() && it->second) {
        // обновляем существующую фигуру на сцене (применяем новые данные из БД)
        domain::Shape* shape = it->second;
        shape->applyData(*shapeData);
        updateConnections();
    } else {
        // если фигуры нет на сцене — создаём заново
        auto shapePtr = domain::ShapeFactory::create(*shapeData);
        if (shapePtr) addShape(shapePtr.release());
    }
}

void GraphicsView::onShapeMoved() {
    // * Обработчик изменения сцены - обновляем позиции всех линий связей
    for (auto* connection : m_connections) {
        connection->updatePosition();
    }
}

void GraphicsView::loadAllShapes() {
    auto allShapes = m_repository->findAll();
    
    for (const auto& shapeData : allShapes) {
        auto shapePtr = domain::ShapeFactory::create(shapeData);
        if (shapePtr) {
            addShape(shapePtr.release());
        }
    }
    
    // после загрузки всех фигур создаём линии связей
    createConnectionItems();
}

void GraphicsView::updateConnections() {
    // удаляем старые линии
    for (auto* connection : m_connections) {
        m_scene->removeItem(connection);
        delete connection;
    }
    m_connections.clear();
    
    // создаём новые
    createConnectionItems();
}

void GraphicsView::createConnectionItems() {
    // проходим по всем фигурам и создаём линии связей
    for (const auto& [fromId, fromShape] : m_shapes) {
        if (!fromShape || !fromShape->isVisible()) {
            continue; // пропускаем невидимые фигуры
        }
        
        const auto& connections = fromShape->connectedShapes();
        
        for (int toId : connections) {
            auto toIt = m_shapes.find(toId);
            if (toIt != m_shapes.end() && toIt->second && toIt->second->isVisible()) {
                // создаём линию связи
                auto* connectionItem = new domain::ConnectionItem(
                    fromId, toId, fromShape, toIt->second
                );
                
                // добавляем на сцену
                m_scene->addItem(connectionItem);
                m_connections.push_back(connectionItem);
            }
        }
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* event) {
    m_isMousePressed = true;
    m_lastMousePos = event->pos();
    
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    if (m_isMousePressed) {
        // * Проверяем, была ли перемещена какая-то фигура под курсором
        QGraphicsItem* item = itemAt(event->pos());
        
        if (item) {
            // itemAt может вернуть дочерний элемент-делегат (эллипс/прямоугольник/полигон)
            // Идём вверх по цепочке родителей, чтобы найти владельца domain::Shape (верхний уровень)
            QGraphicsItem* topItem = item;
            while (topItem && dynamic_cast<domain::Shape*>(topItem) == nullptr) {
                topItem = topItem->parentItem();
            }

            domain::Shape* shape = dynamic_cast<domain::Shape*>(topItem);
            if (shape) {
                // * Сохраняем только позицию в БД (не трогаем другие поля)
                auto shapeOpt = m_repository->findById(shape->id());
                if (shapeOpt) {
                    auto shapeData = *shapeOpt;
                    shapeData.position = shape->scenePos();

                    // сохраняем только позицию (updatePosition)
                    m_repository->updatePosition(shape->id(), shapeData.position);

                    // эмитим сигнал для обновления статус-бара
                    emit shapePositionChanged(shape->id(), shapeData.position);
                }
            }
        }
        
        m_isMousePressed = false;
    }
    
    QGraphicsView::mouseReleaseEvent(event);
}

} // namespace presentation