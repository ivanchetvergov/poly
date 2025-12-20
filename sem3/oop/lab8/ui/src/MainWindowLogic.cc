// MainWindowLogic.cc
#include "MainWindow.h"

#include "Contact.h"
#include "Validator.h"
#include "ContactTableModel.h" 

#include <QMessageBox>
#include <QFileDialog>
#include <QDateEdit>
#include <QLineEdit>
#include <QTableView>
#include <QDataWidgetMapper>
#include <QPushButton> 
#include <QComboBox>

// слот для кнопки "Добавить" / "Сохранить"
void MainWindow::onAddButtonClicked() {
    Contact contact = getCurrentContactFromForm();

    if (!validateContact(contact)) {
        return; // валидация не пройдена
    }
    
    // кнопка используется для двух целей
    if (isInEditMode_) {
        // * режим редактирования:
        // --- 1 ручное обновление телефона в модели
        QModelIndex proxyIndex = tableView_->currentIndex();
        QModelIndex sourceIndex = proxyModel_->mapToSource(proxyIndex);
        
        QString selectedType = phoneTypeComboBox_->currentText();
        QString newNumber = mainPhoneInput_->text().trimmed();

        // получаем изменяемый контакт и обновляем телефон по выбранному типу
        Contact& contactToEdit = contactManager_.getContactsMutable()[sourceIndex.row()];
        contactToEdit.phoneNumbers_[selectedType] = newNumber;
        
        // 2 --- авт обновление остальный полей
        // используем маппер для записи данных из формы обратно в Модель
        if (mapper_->submit()) {
            QMessageBox::information(this, "Успех", "Данные успешно отредактированы.");
        } else {
            // ошибка submit обычно означает проблему с типом данных
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изменения.");
            return;
        }
    } else {
        // * режим добавления:
        // маппер не нужен. используем прямую вставку через модель
        tableModel_->addContact(contact);
        QMessageBox::information(this, "Успех", "Контакт успешно добавлен.");
    }

    // общие действия
    contactManager_.saveToFile("../data/contacts.json");    // сохраняем на диск
    setEditingMode(false);                                  // возвращаемся в режим просмотра
}

// слот для кнопки "Удалить"
void MainWindow::onRemoveButtonClicked() {
    // получаем индекс выбранной строки
    QModelIndex proxyIndex = tableView_->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::information(this, "Удаление", "Пожалуйста, выберите запись для удаления.");
        return;
    }
    
    // преобразуем индекс из Proxy-модели в индекс исходной модели
    int sourceRow = proxyModel_->mapToSource(proxyIndex).row();
    
    // удаляем из исходной модели
    tableModel_->removeContact(sourceRow);
    
    contactManager_.saveToFile("../data/contacts.json");
    // setEditingMode(false); // ! теперь явно остается контакт
}

// cлот для кнопки "Редактировать"
void MainWindow::onEditButtonClicked() {
    QModelIndex proxyIndex = tableView_->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::information(this, "Редактирование", "Пожалуйста, выберите запись для редактирования.");
        return;
    }

    setEditingMode(true);
    mapper_->setCurrentIndex(proxyIndex.row()); // указываем мапперу строку
    
    QMessageBox::information(this, "Редактирование", "Теперь вы можете редактировать данные. Нажмите 'Сохранить' для применения.");
}

void MainWindow::onCancelButtonClicked() {
    mapper_->revert(); // откатываем все изменения в полях, сделанные маппером
    setEditingMode(false);
}

void MainWindow::onSearchTextChanged(const QString& text) {
    QString trimmed = text.trimmed();
    proxyModel_->setFilterKeyColumn(-1);
    proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);

    if (trimmed.isEmpty()) {
        proxyModel_->setFilterRegularExpression(QRegularExpression());
        proxyModel_->setFilterRole(Qt::DisplayRole);
        return;
    }

    // извлекаем только цифры из запроса
    QString digits;
    for (QChar c : trimmed) if (c.isDigit()) digits += c;

    if (!digits.isEmpty()) {
        // ищем по нормализованной строке (в модели — только цифры)
        proxyModel_->setFilterRole(ContactTableModel::NormalizedPhonesRole);
        proxyModel_->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(digits)));
    } else {
        // обычный текстовый поиск по DisplayRole
        proxyModel_->setFilterRole(Qt::DisplayRole);
        proxyModel_->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(trimmed),
                                                                  QRegularExpression::CaseInsensitiveOption));
    }
}

// слот для обработки выбора строки в таблице
void MainWindow::onSelectionChanged() {
    QModelIndex proxyIndex = tableView_->currentIndex();
    bool rowIsSelected = proxyIndex.isValid();
    
    // показываем / скрываем кнопки редактирования и удаления в зависимости от выбора
    // также прячем/показываем только если мы не в режиме редактирования
    if (!isInEditMode_) {
        removeButton_->setVisible(rowIsSelected);
        editButton_->setVisible(rowIsSelected);
        removeButton_->setEnabled(rowIsSelected);
        editButton_->setEnabled(rowIsSelected);
    } else {
        // в режиме редактирования кнопки могут быть скрыты по логике setEditingMode
        removeButton_->setVisible(false);
        editButton_->setVisible(false);
    }
    
    if (rowIsSelected && !isInEditMode_) {
        // синхронизируем маппер с текущей строкой
        mapper_->setCurrentIndex(proxyIndex.row());
        
        // ручное управление телефоном 
        QModelIndex sourceIndex = proxyModel_->mapToSource(proxyIndex);
        const Contact& contact = tableModel_->getContact(sourceIndex.row());
        
        // берем ключ из ComboBox 
        QString selectedType = phoneTypeComboBox_->currentText(); 
        
        // отображаем номер, соответствующий выбранному типу
        QString phoneNumber = contact.phoneNumbers_.value(selectedType);
        mainPhoneInput_->setText(phoneNumber);
    } else if (!rowIsSelected && !isInEditMode_) {
        clearInputFields();
    }
}

void MainWindow::onPhoneTypeChanged(const QString& type) {
    QModelIndex proxyIndex = tableView_->currentIndex();
    if (!proxyIndex.isValid() || isInEditMode_) {
        // если ничего не выбрано или мы в режиме редактирования/добавления, 
        // просто очищаем поле, чтобы не было путаницы.
        mainPhoneInput_->clear();
        return;
    }

    // чтаем данные из модели для выбранной строки и нового типа телефона
    QModelIndex sourceIndex = proxyModel_->mapToSource(proxyIndex);
    const Contact& contact = tableModel_->getContact(sourceIndex.row());
    
    QString phoneNumber = contact.phoneNumbers_.value(type);
    mainPhoneInput_->setText(phoneNumber);
}

void MainWindow::clearInputFields() {
    firstNameInput_->clear();
    lastNameInput_->clear();
    middleNameInput_->clear();
    addressInput_->clear();
    birthDateInput_->setDate(QDate::currentDate());
    emailInput_->clear();
    mainPhoneInput_->clear();
}

Contact MainWindow::getCurrentContactFromForm() const {
    Contact contact;
    QString phoneType = phoneTypeComboBox_->currentText();

    contact.firstName_ = firstNameInput_->text().trimmed();
    contact.lastName_ = lastNameInput_->text().trimmed();
    contact.middleName_ = middleNameInput_->text().trimmed();
    contact.adress_ = addressInput_->text().trimmed();
    contact.birthDate_ = birthDateInput_->date();
    contact.email_ = emailInput_->text().trimmed();
    contact.phoneNumbers_[phoneType] = mainPhoneInput_->text().trimmed();
    return contact;
}

void MainWindow::setEditingMode(bool isEditing) {
    isInEditMode_ = isEditing;
    if (isInEditMode_) {
        addButton_->setText("Сохранить");
        removeButton_->hide();
        editButton_->hide();
        cancelButton_->show();
        tableView_->setEnabled(false);
    } else {
        addButton_->setText("Добавить");
        removeButton_->show();
        editButton_->show();
        cancelButton_->hide();
        tableView_->setEnabled(true);
        clearInputFields();
        tableView_->clearSelection();
    }
}

void MainWindow::onSaveButtonClicked() {
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        if (contactManager_.saveToFile(filePath)) {
            QMessageBox::information(this, "Сохранение", "Данные успешно сохранены.");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
        }
    }
}

void MainWindow::onLoadButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Загрузить файл", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        if (contactManager_.loadFromFile(filePath)) {
            tableModel_->resetTable();
            QMessageBox::information(this, "Загрузка", "Данные успешно загружены.");
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить файл.");
        }
    }
}

bool MainWindow::validateContact(const Contact& contact) {
    bool valid = true;
    QString errors;

    if (!Validator::isValidName(contact.firstName_)) { errors += "Неверное Имя. "; valid = false; }
    if (!Validator::isValidName(contact.lastName_)) { errors += "Неверная Фамилия. "; valid = false; }
    if (!contact.middleName_.isEmpty() && !Validator::isValidName(contact.middleName_)) { 
        errors += "Неверное Отчество. "; valid = false; 
    }
    if (!Validator::isValidBirthDate(contact.birthDate_)) { errors += "Неверная Дата рождения. "; valid = false; }
    if (!Validator::isValidEmail(contact.email_)) { errors += "Неверный Email. "; valid = false; }

    for (const QString& number : contact.phoneNumbers_.values()) {
        if (!Validator::isValidPhoneNumber(number) && !number.isEmpty()) { 
            errors += "Неверный Телефон. "; 
            valid = false;
            break;
        }
    }

    if (!valid) {
        QMessageBox::warning(this, "Ошибка валидации", "Пожалуйста, исправьте:\n" + errors);
    }
    return valid;
}
