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
    auto model = std::make_unique<BasicItemModel>(1);
    auto tester = std::make_unique<QAbstractItemModelTester>(model.get());
    tester.reset();

    //If add items through appendRow methods, no insertRows signals emits and rowCount remains
    //unchanged until you call fetchMore method, if add items through insertRows methods emits
    //insertRows signals and rowCount changes.
    model->clear();
    model->setFetchLimit(1000);
    model->insertRows(0, 1000);
    QVERIFY(model->rowCount() == 1000);
    model->appendRow(new BasicItem);
    QVERIFY(model->rowCount() == 1000);
    model->fetchMore(QModelIndex());
    QVERIFY(model->rowCount() == 1001);
    model->removeRows(10, 10);
    QVERIFY(model->rowCount() == 991);
    model->removeRows(0, 9);
    QVERIFY(model->rowCount() == 982);
    model->insertRows(0, 100);
    QVERIFY(model->rowCount() == 1082);
    model->removeRows(0, 100);
    QVERIFY(model->rowCount() == 982);

    //If item not in the model, then no one signals emits. When item was added to the model it can
    //be removed only through model or through parent item in the model. Model takes ownership of
    //the item.
    BasicItem *outer = new BasicItem;
    outer->appendRow();
    outer->appendRow();
    QVERIFY(outer->rowCount() == 0);
    outer->insertRows(0, 10);
    QVERIFY(outer->rowCount() == 0);
    QVERIFY(outer->childItemCount() == 12);

    model->appendRow(outer);
    QVERIFY(model->rowCount() == 982);
    model->fetchMore(model->index(982, 0));
    QVERIFY(model->rowCount(model->index(982, 0)) == 12);
    QVERIFY(model->rowCount() == 982);
    model->fetchMore(QModelIndex());
    QVERIFY(model->rowCount() == 983);

    //Can't insert or remove items beyond populated items. But you can get index beyond populated
    //items and use it to get data.
    model->appendRow(new BasicItem);
    QVERIFY(model->insertRows(984, 1) == false);
    model->fetchMore(QModelIndex());
    QVERIFY(model->insertRows(984, 1));
    model->appendRow(new BasicItem);
    QVERIFY(model->removeRows(985, 1) == false);
    model->fetchMore(QModelIndex());
    QVERIFY(model->removeRows(985, 1));
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
