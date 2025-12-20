// MainWindow.h
#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>

#include "FiguresWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void on_add_button_clicked();
    void on_remove_button_clicked();

private:
    FiguresWidget* figuresWidget_;      // widget для работы с фигурами
    QComboBox* figureTypeComboBox_;     // выпадающий список
    QPushButton* addButton_;            // кнопка add
    QPushButton* removeButton_;         // кнопка rm
    
    void setupUi();
};