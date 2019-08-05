#include "basicitemmodel.h"
#include "threadsafenode.h"

#include <QtTest>
#include <QAbstractItemModelTester>
#include <future>

class TestBasicItemModel: public QObject
{
    Q_OBJECT
    
private slots:
    void testModel();
    void testThreadSafeRootNode();
};

void TestBasicItemModel::testModel()
{
    BasicItemModel model(1);
    QAbstractItemModelTester tester(&model);
}

void TestBasicItemModel::testThreadSafeRootNode()
{
    //Uncomment to get crash.
    //auto root = new BasicItem;
    auto root = new ThreadsafeNode;
    auto model = std::make_shared<BasicItemModel>(root);

    auto future1 = std::async(std::launch::async, [model]
    {
        for (int i = 0; i < 1000; ++i)
        {
            auto item = new BasicItem;
            item->setText(QString::number(1));
            model->appendRow(item);
        }
    });

    auto future2 = std::async(std::launch::async, [model]
    {
        for (int i = 1000; i < 2000; ++i)
        {
            auto item = new BasicItem;
            item->setText(QString::number(1));
            model->appendRow(item);
        }
    });

    auto future3 = std::async(std::launch::async, [model]
    {
        if (model->rootItem()->rowCount() > 10)
            model->removeRows(model->rootItem()->rowCount() - 10, 5);
    });

    auto future4 = std::async(std::launch::async, [model]
    {
        for (int i = 2000; i < 3000; ++i)
        {
            auto item = new BasicItem;
            item->setText(QString::number(1));
            model->appendRow(item);
        }
    });

    while (root->childItemCount() < 1000)
    {
        root->hasChildItems();
        root->childItem(root->childItemCount() - 1);
        model->itemFromRow(root->childItemCount() - 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    future1.wait();
    future2.wait();
    future3.wait();
    future4.wait();

    QVERIFY(true);
}

QTEST_MAIN(TestBasicItemModel)
#include "testbasicitemmodel.moc"
