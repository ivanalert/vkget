#include <QtTest>

class TestMainWindow: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
};

void TestMainWindow::initTestCase()
{

}

void TestMainWindow::cleanupTestCase()
{

}

QTEST_MAIN(TestMainWindow)
#include "testmainwindow.moc"
