// MainWindow.h
#pragma once

#include <QMainWindow>
#include <QGraphicsView>
#include <QComboBox>
#include <QPushButton>

#include "FiguresScene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void on_add_button_clicked();
    void on_remove_button_clicked();

private:
    FiguresScene* figuresScene_;        // сцена для фигур
    QGraphicsView* graphicsView_;       // представление сцены
    QComboBox* figureTypeComboBox_;     // выпадающий список
    QPushButton* addButton_;            // кнопка add
    QPushButton* removeButton_;         // кнопка rm
    
    void setupUi();
};
