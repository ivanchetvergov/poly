// Contact.h
#pragma once 

#include <QString>
#include <QDate>
#include <QMap>
#include <QUuid>

//* служит моделью QTable
struct Contact {
    // Q_GADGET
public:
    QString firstName_;
    QString lastName_;
    QString middleName_;
    QString adress_;
    QDate birthDate_;
    QString email_;
    QMap<QString, QString> phoneNumbers_;
    QUuid id_ = QUuid::createUuid();
};