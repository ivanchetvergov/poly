#pragma once

#include "graphics_view.h"
#include "shape_model.h"
#include "shape_delegate.h"
#include "infrastructure/database_manager.h"
#include "infrastructure/shape_repository.h"
#include <QMainWindow>
#include <QTableView>
#include <QSplitter>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <memory>

namespace presentation {

/*
 * @brief Главное окно приложения.
 *
 * Класс выполняет роль Контроллера/Представителя, управляя жизненным циклом
 * всех основных компонентов (View, Model, Repository) и обрабатывая
 * пользовательские действия через сигналы и слоты.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onAddShapeClicked();
    void onRemoveShapeClicked();
    void onAddConnectionClicked();
    void onFilterChanged();
    void onRefreshClicked();

private:
    void setupUi();
    void setupConnections();
    void createToolbar();
    void updateGraphicsView();

    // * Infrastructure
    // управляет подключением и жизненным циклом базы данных
    std::unique_ptr<infrastructure::DatabaseManager> m_dbManager;
    // предоставляет интерфейс для чтения/записи данных фигур (Мост между UI и БД)
    std::shared_ptr<infrastructure::ShapeRepository> m_repository;
    
    // * UI Components
    // виджет для отображения фигур и связей (наш кастомный QGraphicsView)
    GraphicsView* m_graphicsView;
    // виджет для отображения данных фигур в виде таблицы
    QTableView* m_tableView;
    // модель данных для QTableView (интерфейс QAbstractTableModel)
    ShapeModel* m_shapeModel;
    // делегат для кастомизации отрисовки ячеек в QTableView
    ShapeDelegate* m_shapeDelegate;
    
    // * Controls (Элементы управления для добавления фигур)
    // выбор типа фигуры (круг, прямоугольник, полигон)
    QComboBox* m_shapeTypeCombo;
    // ввод ширины фигуры
    QSpinBox* m_widthSpin;
    // ввод высоты фигуры
    QSpinBox* m_heightSpin;
    
    // * Controls (Элементы управления для связей)
    // ID фигуры, от которой идет связь
    QSpinBox* m_fromShapeIdSpin;
    // ID фигуры, к которой идет связь
    QSpinBox* m_toShapeIdSpin;
    
    // * Controls (Элементы управления для фильтрации)
    // флажок для скрытия/отображения эллипсов
    QCheckBox* m_filterEllipseCheck;
    // флажок для скрытия/отображения прямоугольников
    QCheckBox* m_filterRectangleCheck;
    // флажок для скрытия/отображения полигонов
    QCheckBox* m_filterPolygonCheck;
    // флажок для фильрации ID
    QCheckBox* m_filterIDActiveCheck; 
    QSpinBox* m_filterIDSpin;

};

} // namespace presentation