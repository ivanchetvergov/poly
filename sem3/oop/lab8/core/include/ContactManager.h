// ContactManager.h
#pragma once 

#include <QList>
#include "Contact.h"

// * класс для управления коллекцией контактов
class ContactManager {
public:
    ContactManager() = default;
    ~ContactManager() = default;

    // * --- CRUD операции ---
    void addContact(const Contact& contact);                 // добавить новую запись
    void removeContact(int index);                           // удалить запись
    void updateContact(int index, const Contact& contact);   // обновить запись

    // * --- геттеры, доступ к данным ---
    const QList<Contact>& getContacts() const;               // для чтения
    QList<Contact>& getContactsMutable();                    // для манипуляций
    int contactCount() const;                                // возвращает число записей
    Contact* getContact(int index);                          // доступ к конакту по индексу

    // * --- работа с данными ---
    bool loadFromFile(const QString& filePath);              // загрузка из файла
    bool saveToFile(const QString& filePath) const;          // сохранение в файл

private:
    QList<Contact> m_contacts; // основная коллекция для всех контактов 
};