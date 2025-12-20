#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Hello World App");
        // void QWidget::setGeometry(int x, int y, int width, int height);
        setGeometry(100, 100, 400, 200);

        QLabel* label = new QLabel("Hello World!", this);
        label->setGeometry(150, 50, 100, 30);

        QPushButton* button = new QPushButton("Exit", this);
        button->setGeometry(150, 100, 100, 30);

        // подключение сигнала (нажатие на кнопку) к слоту (закрытие приложения)
        // (const QObject* sender, const char* signal, const QObject* receiver, const char* method)
        // QApplication::instance() возвращает ед обьект нашего приложения
        QObject::connect(button, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    }

};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MyWidget window;
    window.show();

    return app.exec();
}

// нет заголовка поэтому пришлось вручную.
#include "main.moc"
