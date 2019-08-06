#include "mainwindow.h"

#include <QGuiApplication>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication app(argc, argv);

#ifdef Q_OS_WIN32
    QQuickStyle::setStyle(QStringLiteral("Universal"));
#endif
    MainWindow win;

    return app.exec();
}
