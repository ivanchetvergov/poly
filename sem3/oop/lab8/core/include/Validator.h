// Validator.h
#pragma once

#include <QString>
#include <QDate>

// * класс уилита для валидации данных
// методы статические т.к не зависят от состояния обьекта
class Validator {
public:
    static bool isValidName(const QString& name);
    static bool isValidPhoneNumber(const QString& number);
    static bool isValidEmail(const QString& email);
    static bool isValidBirthDate(const QDate& date);
};

