#include <QApplication>
#include <QVBoxLayout>
#include "MyWidgetCollection.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MyWidgetCollection window;
    window.setWindowTitle("Dynamic Widget Collection");
    window.show();

    return a.exec();
}