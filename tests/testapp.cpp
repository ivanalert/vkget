#include "taskgroup.h"

#include <QtTest>
#include <QtConcurrent>

class TestApp: public QObject
{
    Q_OBJECT

private slots:
    void testTaskGroup();

private:
    static int doSomeWork(const std::atomic<bool> *flag)
    {
        int i = 0;
        while (i < 2000000000)
        {
            if (*flag == 1)
                break;
            ++i;
        }

        return i;
    }
};

void TestApp::testTaskGroup()
{      
    auto group1 = std::make_unique<TaskGroup>();
    auto watcher = new QFutureWatcher<int>(this);
    const auto flag = group1->append(watcher);
    auto future = QtConcurrent::run(TestApp::doSomeWork, flag);
    watcher->setFuture(future);

    //The same watcher.
    group1->append(watcher);

    auto watcher1 = new QFutureWatcher<int>(this);
    const auto flag1 = group1->append(watcher1);
    auto future1 = QtConcurrent::run(TestApp::doSomeWork, flag1);
    watcher1->setFuture(future1);

    auto group2 = std::make_unique<TaskGroup>();
    auto watcher2 = new QFutureWatcher<int>(this);
    const auto flag2 = group2->append(watcher2);
    auto future2 = QtConcurrent::run(TestApp::doSomeWork, flag2);
    watcher2->setFuture(future2);

    auto watcher3 = new QFutureWatcher<int>(this);
    const auto flag3 = group2->append(watcher3);
    auto future3 = QtConcurrent::run(TestApp::doSomeWork, flag3);
    watcher3->setFuture(future3);

    group1->abort(watcher1);
    QCoreApplication::processEvents();
    group1->abort(watcher);
    QCoreApplication::processEvents();
    QVERIFY(group1->empty());
    group2->abort();
    QCoreApplication::processEvents();
    QVERIFY(group2->empty());
}

QTEST_GUILESS_MAIN(TestApp)
#include "testapp.moc"
