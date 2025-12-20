// ContactManager.cc
#include "ContactManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

void ContactManager::addContact(const Contact& contact) {
    m_contacts.append(contact);
}

void ContactManager::removeContact(int index) {
    if (index >= 0 && index < m_contacts.size()) 
        m_contacts.removeAt(index);
}

void ContactManager::updateContact(int index, const Contact& contact) {
    if (index >= 0 && index < m_contacts.size()) {
        m_contacts[index] = contact;
    }
}

const QList<Contact>& ContactManager::getContacts() const {
    return m_contacts;
}

bool ContactManager::saveToFile(const QString& filePath) const {
    QJsonArray contactArray;
    // формируем JSON
    for (const auto& contact : m_contacts) {
        QJsonObject contactObject;
        // записываем поля в json
        contactObject["firstName"] = contact.firstName_;
        contactObject["lastName"] = contact.lastName_;
        contactObject["middleName"] = contact.middleName_;
        contactObject["address"] = contact.adress_; 
        contactObject["birthDate"] = contact.birthDate_.toString(Qt::ISODate);
        contactObject["email"] = contact.email_;
        // добавляем номера
        QJsonObject phonesObject;
        for (auto it = contact.phoneNumbers_.constBegin(); 
            it != contact.phoneNumbers_.constEnd(); ++it) {
            phonesObject[it.key()] = it.value();
        }
        contactObject["phoneNumbers"] = phonesObject;
        // добавляем в массив
        contactArray.append(contactObject);
    }
    // настраиваем 
    QJsonDocument doc(contactArray);
    QFile file(filePath); // открываем файлив
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson());
        file.close();
        return true;
    }

    qWarning() << "Could not save file:" << file.errorString();
    return false;
}

bool ContactManager::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for reading:" << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "Invalid JSON document.";
        return false;
    }
    
    m_contacts.clear();
    QJsonArray contactArray = doc.array();
    for (const QJsonValue& value : contactArray) {
        QJsonObject contactObject = value.toObject();
        Contact contact;
        contact.firstName_ = contactObject["firstName"].toString(); 
        contact.lastName_ = contactObject["lastName"].toString();   
        contact.middleName_ = contactObject["middleName"].toString(); 
        contact.adress_ = contactObject["address"].toString();     
        contact.birthDate_ = QDate::fromString(contactObject["birthDate"].toString(), Qt::ISODate); 
        contact.email_ = contactObject["email"].toString();         
        
        QJsonObject phonesObject = contactObject["phoneNumbers"].toObject();
        for (const QString& key : phonesObject.keys()) {
            contact.phoneNumbers_[key] = phonesObject[key].toString(); 
        }
        m_contacts.append(contact);
    }
    return true;
}

QList<Contact>& ContactManager::getContactsMutable() {
    return m_contacts;
}

Contact* ContactManager::getContact(int index) {
    if (index >= 0 && index < m_contacts.size()) {
        return &m_contacts[index];
    }
    return nullptr; 
}

int ContactManager::contactCount() const {
    return m_contacts.size();
}