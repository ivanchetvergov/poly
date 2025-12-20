// MyWidgetCollection.hpp
#pragma once 

#include <QWidget>
#include <QVector>

class QLabel;
class QSlider;
class QScrollBar;
class QSpinBox;

class MyWidgetCollection : public QWidget {
    Q_OBJECT // MOC (Meta Obj Comp)

public:
    MyWidgetCollection(QWidget* parent = nullptr);
    void addWidget(const QString& type);
    void rmLastWidget();

public slots: 
    void connectAllWidgets();

private:
    void setupUi();
    void connectWidgets(QWidget* widget1, QWidget* widget2);
    void debugConnectionInfo();

    QVector<QWidget*> widgets_;
};     