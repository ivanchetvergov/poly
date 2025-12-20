// ContactTableModel.h
#pragma once
#include "Contact.h"
#include "ContactManager.h"

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>

//* класс-адаптер ContactManager -> QTableView
class ContactTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // констрктор принимает указатель на contactManager (источник данных) 
    explicit ContactTableModel(ContactManager* contactManager, QObject* parent = nullptr);

    //* --- обязательные методы QAbstractTableModel (структура и чтение) ---    

    // возвращает количество строк (записей Contact)
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;             
    // возвращает количество столбцов (полей Contact)
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;          
    // предоставляет данные для ячейки (index) в заданной роли (role)
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override; 
    // возвращает название стобцов
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //* --- обязательные методы QAbstractTableModel (редактирование) ---
    
    // записывает новое значение (value) в ячейку (index) и обновляет модель
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    // определяет свойства ячейки (можно ли выбирать, редактировать и т.д.)
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    //* --- методы-обертки CRUD операций (синхронизация модели) ---
    void addContact(const Contact& contact);
    void removeContact(int index);
    void updateContact(int index, const Contact& contact);

    // * --- геттеры ---
    const Contact& getContact(int row) const;

    // * --- вспомогательные методы ---
    QString normalizeDigits(const QString& s);
    QString normalizeDigits(const QString& s) const;

    // *статические константы для пользовательских ролей
    static constexpr int NormalizedPhonesRole = Qt::UserRole + 1;
    static constexpr int ContactIdRole = Qt::UserRole + 2;

public slots:
    // слот для полного сброса и перерисовки всей таблицы
    void resetTable();

private:
    // указатель на источник данных
    ContactManager* contactManager_;

};