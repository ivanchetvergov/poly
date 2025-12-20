// MainWindow.cc
#include "MainWindow.h"
#include "ContactTableModel.h"
#include "ContactManager.h"
#include "Contact.h"

#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QItemSelectionModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateEdit>
#include <QComboBox>

// конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // * --- 1 инициализация UI 
    setupUi();

    // * --- 2 инициадизация модели и покси
    // базовая модель
    tableModel_ = new ContactTableModel(&contactManager_, this);
    
    // прокси. отвечает за сорт и фильтр
    proxyModel_ = new QSortFilterProxyModel(this);
    proxyModel_->setSourceModel(tableModel_);
    proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);

    // * --- 3 настойка вида
    tableView_->setModel(proxyModel_);
    tableView_->setSortingEnabled(true);
    
    // * --- 4 связь сигналов и слотов
    // --- CRUD --- 
    connect(addButton_, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(removeButton_, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);
    connect(editButton_, &QPushButton::clicked, this, &MainWindow::onEditButtonClicked);
    connect(cancelButton_, &QPushButton::clicked, this, &MainWindow::onCancelButtonClicked);
    connect(phoneTypeComboBox_, &QComboBox::currentTextChanged, this, &MainWindow::onPhoneTypeChanged);
    
    // -- поиск ---
    connect(searchInput_, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    // --- выбор строки
    connect(tableView_->selectionModel(), &QItemSelectionModel::currentChanged, 
            this, &MainWindow::onSelectionChanged);
    
    // * 5 --- работа с файлами
    QPushButton* saveButton = new QPushButton("Сохранить файл");
    QPushButton* loadButton = new QPushButton("Загрузить файл");

    // слой для файловых кнопок
    QHBoxLayout* fileButtonsLayout = new QHBoxLayout();
    fileButtonsLayout->addWidget(saveButton);
    fileButtonsLayout->addWidget(loadButton);

    // добавляем их на основной слой
    mainLayout_->insertLayout(0, fileButtonsLayout);

    // подключаем сигналы
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);

    // * 6 --- настройка QDataWidgetMapper ---
    setupMapper();

    phoneTypeComboBox_->setCurrentText("Домашний");


    // * 7 --- начальная загрузка данных ---
    if (contactManager_.loadFromFile("../data/contacts.json")) 
        tableModel_->resetTable();    
}

void MainWindow::setupMapper() {
    // автоматический маппер для связывания столбцов модели с виджетами
    mapper_ = new QDataWidgetMapper(this);
    mapper_->setModel(proxyModel_); // Mapper всегда работает с прокси-моделью

    // запрещаем автосохранение
    mapper_->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    
    // настройка столбцов
    mapper_->addMapping(firstNameInput_, 0); 
    mapper_->addMapping(lastNameInput_, 1); 
    mapper_->addMapping(middleNameInput_, 2); 
    mapper_->addMapping(addressInput_, 3);
    mapper_->addMapping(birthDateInput_, 4);
    mapper_->addMapping(emailInput_, 5);

}