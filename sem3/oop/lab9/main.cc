#include "presentation/main_window.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("Graphics Primitives");
    app.setApplicationVersion("1.0.0");
    
    try {
        presentation::MainWindow mainWindow;
        mainWindow.show();
        
        return app.exec();
        
    } catch (const std::exception& e) {
        qCritical() << "Fatal error:" << e.what();
        return 1;
    }
}