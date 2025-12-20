// MainWindowUI.cc
#include "MainWindow.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QComboBox>

void MainWindow::setupUi()
{
    // устанавливаем центральный виджет
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // главный вертикальный слой
    mainLayout_ = new QVBoxLayout(centralWidget);
    
    // * --- 1. секция формы ввода (Grid Layout) ---
    QGridLayout* inputLayout = new QGridLayout();
    
    //инициализация виджетов ввода
    lastNameInput_ = new QLineEdit();
    firstNameInput_ = new QLineEdit();
    middleNameInput_ = new QLineEdit();
    addressInput_ = new QLineEdit();
    // др
    birthDateInput_ = new QDateEdit(QDate::currentDate());
    birthDateInput_->setDisplayFormat("dd.MM.yyyy"); 
    birthDateInput_->setCalendarPopup(true);         
    
    emailInput_ = new QLineEdit();
    
    mainPhoneInput_ = new QLineEdit();
    phoneTypeComboBox_ = new QComboBox();
    phoneTypeComboBox_->addItem("Рабочий");
    phoneTypeComboBox_->addItem("Домашний");
    phoneTypeComboBox_->addItem("Служебный");

    // размещение полей в сетке (QLabel, QLineEdit)
    inputLayout->addWidget(new QLabel("Фамилия:"), 0, 0);
    inputLayout->addWidget(lastNameInput_, 0, 1);
    inputLayout->addWidget(new QLabel("Имя:"), 1, 0);
    inputLayout->addWidget(firstNameInput_, 1, 1);
    inputLayout->addWidget(new QLabel("Отчество:"), 2, 0);
    inputLayout->addWidget(middleNameInput_, 2, 1);
    inputLayout->addWidget(new QLabel("Адрес:"), 3, 0);
    inputLayout->addWidget(addressInput_, 3, 1);
    inputLayout->addWidget(new QLabel("Дата рождения:"), 4, 0);
    inputLayout->addWidget(birthDateInput_, 4, 1);
    inputLayout->addWidget(new QLabel("Email:"), 5, 0);
    inputLayout->addWidget(emailInput_, 5, 1);
    
    // размещение телефона: тип и номер в одной строке
    QHBoxLayout* phoneInputRow = new QHBoxLayout();
    phoneInputRow->addWidget(phoneTypeComboBox_);
    phoneInputRow->addWidget(mainPhoneInput_);
    inputLayout->addWidget(new QLabel("Телефон:"), 6, 0);
    inputLayout->addLayout(phoneInputRow, 6, 1);

    mainLayout_->addLayout(inputLayout);
    
    // * --- 2 секция кнопок (Horizontal Layout)
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addButton_ = new QPushButton("Добавить");
    removeButton_ = new QPushButton("Удалить");
    editButton_ = new QPushButton("Редактировать");
    cancelButton_ = new QPushButton("Отмена");

    // инициализация состояния кнопок
    removeButton_->setEnabled(false); // нельзя удалить пока ничего не выбрано
    editButton_->setEnabled(false);   // нельзя редактировать пока ничего не выбрано
    cancelButton_->hide();            // скрыта до начала редактирования/добавления
    
    buttonLayout->addWidget(addButton_);
    buttonLayout->addWidget(removeButton_);
    buttonLayout->addWidget(editButton_);
    buttonLayout->addWidget(cancelButton_);
    mainLayout_->addLayout(buttonLayout);

    // * 3 секция поиска (Horizontal Layout)
    QHBoxLayout* searchLayout = new QHBoxLayout();
    
    // поиск
    searchInput_ = new QLineEdit();
    searchInput_->setPlaceholderText("Поиск по имени по всем полям");
    searchLayout->addWidget(new QLabel("Поиск:"));
    searchLayout->addWidget(searchInput_);

    mainLayout_->addLayout(searchLayout);

    // * --- 4 секция таблицы (View)
    tableView_ = new QTableView();
    tableView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // растянуть столбцы
    tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);            // выделение только целых строк
    tableView_->setSelectionMode(QAbstractItemView::SingleSelection);           // только одна строка может быть выбрана
    tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);             // запрещаем редактирование в самой таблице

    mainLayout_->addWidget(tableView_);
}