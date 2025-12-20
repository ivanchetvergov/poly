// ContactTableModel.cc 
#include "ContactTableModel.h"
#include <QRegularExpression>

ContactTableModel::ContactTableModel(ContactManager* contactManager, QObject* parent)
    : QAbstractTableModel(parent),
      contactManager_(contactManager) {}

int ContactTableModel::rowCount(const QModelIndex& parent) const {
    return contactManager_->getContacts().size();
}

int ContactTableModel::columnCount(const QModelIndex& parent) const {
    return 7;
}

QString ContactTableModel::normalizeDigits(const QString& s) const {
    QString out;
    out.reserve(s.size());
    for (QChar c : s) if (c.isDigit()) out += c;
    return out;
}

// отвечает за чтение данных
// * QVariant универсальный контейнер
QVariant ContactTableModel::data(const QModelIndex &index, int role) const {
    // базовая проверка
    if (!index.isValid() || index.row() >= contactManager_->getContacts().size()) {
        return QVariant();
    }
    // получаем контакты
    const QList<Contact>& contacts = contactManager_->getContacts();
    const Contact& contact = contacts.at(index.row());

    // пользовательская роль для нормализованных телефонов
    if (role == NormalizedPhonesRole) {
        const Contact &c = getContact(index.row());
        QString allDigits;
        for (const QString &num : c.phoneNumbers_.values()) {
            QString d = normalizeDigits(num);
            if (!d.isEmpty()) {
                if (!allDigits.isEmpty()) allDigits += ' ';
                allDigits += d;
            }
        }
        return allDigits;
    }

    // роли это как ключи QTableView
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return contact.firstName_;
            case 1: return contact.lastName_;
            case 2: return contact.middleName_;
            case 3: return contact.adress_;
            case 4: return contact.birthDate_;
            case 5: return contact.email_;
            case 6: return contact.phoneNumbers_.values().join(", "); 
        }
    }
    // // пользовательская роль ContactIdRole (256: int)
    // if (role == ContactIdRole) {
    //     return contact.id_; // возвращаем уникальный ID
    // }

    return QVariant();
}

// метод который возвращает название столбцовв
QVariant ContactTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    // проверка роли и ориентации, нужна горизонт -> (для заголовков)
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }
    
    switch (section) {
        case 0: return "Имя";
        case 1: return "Фамилия";
        case 2: return "Отчество";
        case 3: return "Адрес";
        case 4: return "Дата рождения";
        case 5: return "Email";
        case 6: return "Телефон";
        default: return QVariant();
    }
}

// метод отвечающий за запись даты
bool ContactTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    // проверка возможности редактирования
    if (index.isValid() && role == Qt::EditRole) {
        // получаем ссылку на контакт, а не копию
        QList<Contact>& contacts = contactManager_->getContactsMutable(); 
        Contact& contact = contacts[index.row()];
        
        switch (index.column()) {
            case 0: contact.firstName_ = value.toString(); break;
            case 1: contact.lastName_ = value.toString(); break;
            case 2: contact.middleName_ = value.toString(); break;
            case 3: contact.adress_ = value.toString(); break;
            case 4: contact.birthDate_ = value.toDate(); break;
            case 5: contact.email_ = value.toString(); break;
            case 6: contact.phoneNumbers_["Рабочий"] = value.toString(); break;
            default: return false;
        }
        // запускаем сигнал от QTableView
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

// флаги отвечают за права юзера
Qt::ItemFlags ContactTableModel::flags(const QModelIndex& index) const {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void ContactTableModel::addContact(const Contact& contact) {
    int newRow = contactManager_->getContacts().size();
/* 
    beginInsertRows и endInsertRows часть интерфейса QAbstractTableModel, 
    которая обеспечивает синхронизацию между ContactTableModel и QTableView 
    согласно архитектуре Model/View
*/
    beginInsertRows(QModelIndex(), newRow, newRow);
    contactManager_->addContact(contact);
    endInsertRows();
}

void ContactTableModel::removeContact(int index) {
    if (index >= 0 && index < contactManager_->getContacts().size()) {
        // аналогично связь Model/View
        beginRemoveRows(QModelIndex(), index, index);
        contactManager_->removeContact(index);
        endRemoveRows();
    }
}

void ContactTableModel::updateContact(int index, const Contact& contact) {
    if (index >= 0 && index < contactManager_->getContacts().size()) {
        contactManager_->updateContact(index, contact);
        // вызываем сигнал
        emit dataChanged(this->index(index, 0), 
            this->index(index, columnCount() - 1));
    }
}

const Contact& ContactTableModel::getContact(int row) const {
    return *contactManager_->getContact(row);
}

void ContactTableModel::resetTable() {
    beginResetModel();
    endResetModel();
}