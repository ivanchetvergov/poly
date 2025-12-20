// FiguresScene.cc
#include "FiguresScene.h"
#include "RectItem.h"
#include "EllipseItem.h"
#include "TriagItem.h"

FiguresScene::FiguresScene(QObject* parent)
    : QGraphicsScene(parent) 
{
    // Устанавливаем размер сцены
    setSceneRect(0, 0, 800, 600);
}

void FiguresScene::addRectangle(const QPointF& position, const QColor& color, 
                                 qreal width, qreal height) {
    RectItem* rect = new RectItem(position, color, width, height);
    rect->setZValue(currentZValue_++);
    addItem(rect);
    figuresOrder_.append(rect);
}

void FiguresScene::addEllipse(const QPointF& position, const QColor& color, 
                               qreal width, qreal height) {
    EllipseItem* ellipse = new EllipseItem(position, color, width, height);
    ellipse->setZValue(currentZValue_++);
    addItem(ellipse);
    figuresOrder_.append(ellipse);
}

void FiguresScene::addTriangle(const QColor& color, const QPointF& p1, 
                                const QPointF& p2, const QPointF& p3) {
    TriagItem* triag = new TriagItem(color, p1, p2, p3);
    triag->setZValue(currentZValue_++);
    addItem(triag);
    figuresOrder_.append(triag);
}

void FiguresScene::removeLastFigure() {
    if (!figuresOrder_.isEmpty()) {
        QGraphicsItem* lastItem = figuresOrder_.takeLast();
        removeItem(lastItem);
        delete lastItem;
    }
}

void FiguresScene::clearAllFigures() {
    // Удаляем все фигуры
    for (QGraphicsItem* item : figuresOrder_) {
        removeItem(item);
        delete item;
    }
    figuresOrder_.clear();
    currentZValue_ = 0;
}

void FiguresScene::bringToFront(QGraphicsItem* item) {
    if (item) {
        // Устанавливаем новый максимальный Z-Value
        item->setZValue(currentZValue_++);
        
        // Перемещаем элемент в конец списка 
        figuresOrder_.removeOne(item);
        figuresOrder_.append(item);
    }
}
