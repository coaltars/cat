#include <QApplication>
#include <QStyleFactory>
#include "ui/main.h"
#include "core/theme.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}