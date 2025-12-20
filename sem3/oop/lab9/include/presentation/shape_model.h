#pragma once

#include "infrastructure/shape_repository.h"
#include <QAbstractTableModel>
#include <QSet>
#include <memory>

namespace presentation {
/*
 * @brief Модель данных для QTableView (Qt Abstract Table Model).
 *
 * Выступает в качестве адаптера между ShapeRepository и QTableView,
 * отвечая за предоставление, форматирование и фильтрацию данных фигур
 * для отображения в таблице.
 */
class ShapeModel : public   {
    Q_OBJECT

public:
    enum Column {
        IdColumn = 0,
        TypeColumn,
        CountColumn,
        ConnectionsColumn,
        ColumnCount
    };

    explicit ShapeModel(std::shared_ptr<infrastructure::ShapeRepository> repository,
                       QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, 
                                     int role = Qt::DisplayRole) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());
    void refresh();
    
    void setTypeFilter(domain::ShapeType type, bool visible);
    void setTypeFilters(bool ellipseVisible, bool rectangleVisible, bool polygonVisible);
    void clearFilters();
    [[nodiscard]] domain::ShapeData getShapeData(int row) const;

signals:
    void shapeVisibilityChanged();

private slots:
    void onShapeAdded(int id);   // обработка добавления фигуры в репозиторий
    void onShapeRemoved(int id); // обработка удаления фигуры из репозитория

private:
    void updateFilteredShapes();
    [[nodiscard]] int getCountForType(domain::ShapeType type) const;
    // форматирует вектор ID связей в строку (например: "1, 3, 6")
    [[nodiscard]] QString getConnectionsString(const std::vector<int>& connections) const;

    // указатель на репозиторий данных
    // отвечает за доступ к БД и бизнес-логике
    std::shared_ptr<infrastructure::ShapeRepository> m_repository;
    
    // полный список всех фигур, загруженных из репозитория
    // является основным источником данных для модели (кэш)
    std::vector<domain::ShapeData> m_shapes;
    
    // список фигур, отфильтрованный на основе m_hiddenTypes
    // этот список используется для отображения в QTableView.
    std::vector<domain::ShapeData> m_filteredShapes;
    
    // набор типов фигур (ShapeType), которые должны быть скрыты в таблиц
    QSet<domain::ShapeType> m_hiddenTypes;

    // мапа формата: {ShapeType::RECTANGLE: 15, ShapeType::ELLIPSE: 7, ...}
    std::map<domain::ShapeType, int> m_typeCounts;
};

} // namespace presentation