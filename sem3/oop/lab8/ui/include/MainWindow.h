// MainWindow.h
#pragma once

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include "ContactManager.h"
#include "ContactTableModel.h"

// ! предварительное объявление классов
class QPushButton;
class QLineEdit;
class QDateEdit;
class QComboBox;
class QVBoxLayout;
class QTableView;
class QDataWidgetMapper;
class QListWidget;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    // * --- слоты для кнопок (CRUD) ---
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onEditButtonClicked();
    void onSaveButtonClicked();
    void onLoadButtonClicked();
    void onCancelButtonClicked();

    // * --- слоты для взаимодействия с таблицей и поиском ---
    // обработка ввода в поле поиска
    void onSearchTextChanged(const QString& text); 
    // обработка выбора строки в QTableView
    void onSelectionChanged();       
    // выбор телефона 
    void onPhoneTypeChanged(const QString& type);              

private:
    // статическая константа для пользовательской роли
    static constexpr int ContactIdRole = Qt::UserRole + 1; 

    // * --- инициализация и служебные методы ---
    void setupUi();                                 // создание и размещение всех виджетов
    void setupMapper();                             // настройка QDataWidgetMapper для связывания полей
    void clearInputFields();                        // очистка всех полей ввода
    Contact getCurrentContactFromForm() const;      // сбор данных из полей в объект Contact
    bool validateContact(const Contact& contact);   // валидация через класс Validator

    // * --- управление состоянием ---
    bool isInEditMode_ = false;                     // флаг: находимся ли мы в режиме редактирования/добавления
    void setEditingMode(bool isInEditMode);         // переключает доступность виджетов (режим: просмотр/редактирование)

    // * --- основные компоненты ядра и модели ---
    ContactManager contactManager_;                 // владелец всех данных 
    ContactTableModel* tableModel_;                 // базовая модель (адаптер Manager -> Proxy)
    QSortFilterProxyModel* proxyModel_;             // прокси-модель для сортировки и поиска 
    QTableView* tableView_;                         // виджет для отображения данных  
    
    // * --- виджеты для ввода данных ---
    QDataWidgetMapper* mapper_;                     // инструмент для авт. синх. формы и таблицы при редакте
    
    QLineEdit* firstNameInput_;
    QLineEdit* lastNameInput_;
    QLineEdit* middleNameInput_;
    QLineEdit* addressInput_;
    QDateEdit* birthDateInput_;
    QLineEdit* emailInput_;

    QLineEdit* mainPhoneInput_;                     // основное поле для ввода номера
    QComboBox* phoneTypeComboBox_;                  // выбор типа номера (рабочий, домашний и т.д)
    
    // * --- кнопки ---
    QPushButton* addButton_;
    QPushButton* removeButton_;
    QPushButton* editButton_;
    QPushButton* cancelButton_;

    // * --- поиск и сортировка ---
    QLineEdit* searchInput_;

    // * --- слои ---
    QVBoxLayout* mainLayout_;                       // главный слой окна
};