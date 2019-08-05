#include "mainwindow.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication app(argc, argv);
    MainWindow win;

    return app.exec();
}
