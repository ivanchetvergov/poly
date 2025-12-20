#include "Validator.h"
#include <QRegularExpression>
#include <QDate>

bool Validator::isValidName(const QString& name) {
    QString trimmedName = name.trimmed(); // убрали пробелы
    
    // проверка на пустой ввод
    if (trimmedName.isEmpty()) return false;
    
    // проверка на наличие дефиса с концов
    if (trimmedName.startsWith('-') || trimmedName.endsWith('-')) return false; 

    //* регулярное выражениие:
    // ^[А-ЯA-Z] - начинается с заглавной буквы
    // [А-Яа-яA-Za-z-]* - далее могут идти буквы пробел или дефис (любое количество)
    // [А-Яа-яA-Za-z0-9]$ - заканчивается на букву или цифру
    QRegularExpression re("^[А-ЯA-Z][А-Яа-яA-Za-z -]*[А-Яа-яA-Za-z0-9]$");
    // проверяем на мэтч и возращаем bool
    return re.match(trimmedName).hasMatch();
}

bool Validator::isValidPhoneNumber(const QString& number) {
    // ^(\\+)? - необязательный плюс в начале
    // [\\s\\d()-]+$ - одна или более цифр, пробелов, скобок или дефисов
    // \\s пробел и тд. \\d цифра
    QRegularExpression re("^(\\+)?[\\s\\d()-]+$");
    return re.match(number).hasMatch();
}

bool Validator::isValidEmail(const QString& email) {
    QString trimmedEmail = email.trimmed();
    if (trimmedEmail.isEmpty()) return false; 
    
    // ^[a-zA-Z0-9]+ - имя пользователя из букв и цифр
    // [a-zA-Z0-9]+\\. - домен из букв и цифр с точкой
    // [a-zA-Z0-9]+$ - зона домена
    QRegularExpression re("^[a-zA-Z0-9]+@[a-zA-Z0-9]+\\.[a-zA-Z0-9]+$");
    return re.match(trimmedEmail).hasMatch();
}

bool Validator::isValidBirthDate(const QDate& date) {
    return date.isValid() && date < QDate::currentDate();
}