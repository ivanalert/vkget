#include "mainwindow.h"

#include <QGuiApplication>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("VKGet"));
    QCoreApplication::setOrganizationName(QStringLiteral("ivanalert"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("ivanalert@mail.ru"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication app(argc, argv);

#ifdef Q_OS_WIN32
    QQuickStyle::setStyle(QStringLiteral("Universal"));
#endif
    MainWindow win;

    return app.exec();
}
