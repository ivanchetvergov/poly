#include "MyWidgetCollection.hpp"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QScrollBar>
#include <QSpinBox>
#include <QMetaObject>


MyWidgetCollection::MyWidgetCollection(QWidget* parent)
    :  QWidget(parent) { setupUi(); }

void MyWidgetCollection::setupUi() { 
    auto layout = new QVBoxLayout(this); // макет для верт расположения виджетов
    // все экземпляры кнопок
    auto addSliderBtn = new QPushButton("add QSlider");
    auto addScrollBarBtn = new QPushButton("add QScrollBar");
    auto addSpinBoxBtn = new QPushButton("add QSpinBox");
    auto addLabelBtn = new QPushButton("add QLabel");
    auto connectBtn = new QPushButton("connect all widgets");
    auto rmBtn =  new QPushButton("rm last");


    layout->addWidget(addSliderBtn);
    layout->addWidget(addScrollBarBtn);
    layout->addWidget(addSpinBoxBtn);
    layout->addWidget(addLabelBtn);
    layout->addWidget(connectBtn);
    layout->addWidget(rmBtn);

    // связали сигналы и слоты
    // линковка путем указателей на ф-ии и вызова лямбды ф-ии
    connect(addSliderBtn, &QPushButton::clicked, this, [this]() { addWidget("QSlider"); });
    connect(addScrollBarBtn, &QPushButton::clicked, this, [this]() { addWidget("QScrollBar"); });
    connect(addSpinBoxBtn, &QPushButton::clicked, this, [this]() { addWidget("QSpinBox"); });
    connect(addLabelBtn, &QPushButton::clicked, this, [this]() { addWidget("QLabel"); });
    connect(rmBtn, &QPushButton::clicked, this, [this]() { rmLastWidget(); });

    // линковка сигнала со слотом 
    connect(connectBtn, &QPushButton::clicked, this, &MyWidgetCollection::connectAllWidgets);

    setMinimumSize(400, 300);
}

void MyWidgetCollection::addWidget(const QString& type) {
    QWidget* newWidget = nullptr;

    const int min = 0;
    const int max = 100;

    // this  делает наш класс родителем для каждого виджета,
    // что обеспечивает автоматическое управление памятью.
    // bool QObject::setProperty(const char* name, const QVariant& value);
    if (type == "QLabel") {
        newWidget = new QLabel("Label: 0", this);
    } else if (type == "QSlider") {
        newWidget = new QSlider(Qt::Horizontal, this);
        newWidget->setProperty("minimum", min);
        newWidget->setProperty("maximum", max);
    } else if (type == "QScrollBar") {
        newWidget = new QScrollBar(Qt::Horizontal, this);
        newWidget->setProperty("minimum", min);
        newWidget->setProperty("maximum", max);
    } else if (type == "QSpinBox") {
        newWidget = new QSpinBox(this);
        newWidget->setProperty("minimum", min);
        newWidget->setProperty("maximum", max);
    }

    if (newWidget) {
        widgets_.append(newWidget);
        static_cast<QVBoxLayout*>(layout())->addWidget(newWidget);
        newWidget->show();
    }
}

void MyWidgetCollection::rmLastWidget(){
    // QWidget* lastWidget = widgets_[widgets_.size() - 1];
    QWidget* lastWidget = widgets_.last();

    delete lastWidget;
}

void MyWidgetCollection::connectWidgets(QWidget* w1, QWidget* w2) {
    // достаем имя класса обьектов
    QString class1 = w1->metaObject()->className();
    QString class2 = w2->metaObject()->className();

    // cоединяем w1 (источник) с w2 (приемник)
    if (class1 == "QSlider"    || 
        class1 == "QScrollBar" || 
        class1 == "QSpinBox") {
        
        // флаг Qt::UniqueConnection предотвращает дублирование
        // QObject::connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()));
        // неизвестны типы данных поэтому остается коннект напрямую
        // ч-з макросы SIGNAL и SLOT (создают строки "ключи")
        if (class2 == "QLabel") {
            connect(w1, SIGNAL(valueChanged(int)), w2, 
                SLOT(setNum(int)), Qt::UniqueConnection);
        } else if ( class2 == "QSlider" || 
                    class2 == "QScrollBar" || 
                    class2 == "QSpinBox") {
            connect(w1, SIGNAL(valueChanged(int)), w2, 
                SLOT(setValue(int)), Qt::UniqueConnection);
        }
    }
}


void MyWidgetCollection::connectAllWidgets() {
    qDebug() << "Connecting all widgets...";

    for (int i = 0; i < widgets_.size(); ++i) {
        for (int j = i + 1; j < widgets_.size(); ++j) {
            QWidget* w1 = widgets_[i];
            QWidget* w2 = widgets_[j];

            connectWidgets(w1, w2);
            connectWidgets(w2, w1);
        }
    }

    qDebug() << "All widgets connected.";
    debugConnectionInfo();
}

void MyWidgetCollection::debugConnectionInfo() {
    qDebug() << "--- Connection Debugging ---";
    int totalConnections = 0;
    for (QWidget* w : widgets_) {
        qDebug() << "Widget" << w->metaObject()->className() << "at" << w << "has connections.";
    }        
    // QObject::dumpObjectInfo();
    qDebug() << "-----------------------------";
}

// void MyWidgetCollection::connectWidgets(QWidget* w1, QWidget* w2) {
//     QSlider* slider1 = qobject_cast<QSlider*>(w1);
//     QScrollBar* scrollbar1 = qobject_cast<QScrollBar*>(w1);
//     QSpinBox* spinbox1 = qobject_cast<QSpinBox*>(w1);
//     QLabel* label1 = qobject_cast<QLabel*>(w1);

//     QSlider* slider2 = qobject_cast<QSlider*>(w2);
//     QScrollBar* scrollbar2 = qobject_cast<QScrollBar*>(w2);
//     QSpinBox* spinbox2 = qobject_cast<QSpinBox*>(w2);
//     QLabel* label2 = qobject_cast<QLabel*>(w2);

//     // connect from w1 to w2
//     if (slider1) {
//         if (slider2) connect(slider1, &QSlider::valueChanged, slider2, &QSlider::setValue);
//         else if (scrollbar2) connect(slider1, &QSlider::valueChanged, scrollbar2, &QScrollBar::setValue);
//         else if (spinbox2) connect(slider1, &QSlider::valueChanged, spinbox2, &QSpinBox::setValue);
//         else if (label2) {
//             connect(slider1, &QSlider::valueChanged, this, [label2](int value){ 
//                 label2->setNum(static_cast<double>(value)); 
//             });
//         }
//     } else if (scrollbar1) {
//         if (slider2) connect(scrollbar1, &QScrollBar::valueChanged, slider2, &QSlider::setValue);
//         else if (scrollbar2) connect(scrollbar1, &QScrollBar::valueChanged, scrollbar2, &QScrollBar::setValue);
//         else if (spinbox2) connect(scrollbar1, &QScrollBar::valueChanged, spinbox2, &QSpinBox::setValue);
//         else if (label2) {
//             connect(scrollbar1, &QScrollBar::valueChanged, this, [label2](int value){
//                 label2->setNum(static_cast<double>(value));
//             });
//         }
//     } else if (spinbox1) {
//         if (slider2) connect(spinbox1, &QSpinBox::valueChanged, slider2, &QSlider::setValue);
//         else if (scrollbar2) connect(spinbox1, &QSpinBox::valueChanged, scrollbar2, &QScrollBar::setValue);
//         else if (spinbox2) connect(spinbox1, &QSpinBox::valueChanged, spinbox2, &QSpinBox::setValue);
//         else if (label2) {
//             connect(spinbox1, &QSpinBox::valueChanged, this, [label2](int value){
//                 label2->setNum(static_cast<double>(value));
//             });
//         }
//     }
// }