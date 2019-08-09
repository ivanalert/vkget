#ifndef THREADSAFENODE_H
#define THREADSAFENODE_H

#include "basicitem.h"

#include <QMutex>

//Model fills in different threads.
//Model items creates and fills in different threads.
//Model data writes and reads in one thread.
//Need proptect only node items and only insertRows and removeRows functions.
class ThreadsafeNode final : public BasicItem
{

public:
    explicit ThreadsafeNode(const QVector<QMap<int, QVariant>> &data,
                            BasicItem *parent = nullptr)
        : BasicItem(data, parent)
    {

    }

    explicit ThreadsafeNode(int columns = 1, BasicItem *parent = nullptr)
        : BasicItem(columns, parent)
    {

    }

    ThreadsafeNode(ThreadsafeNode &&other) noexcept = delete;
    ThreadsafeNode& operator=(ThreadsafeNode &&other) noexcept = delete;
    ~ThreadsafeNode() override = default;

    // BasicItem interface
    BasicItem *clone() const override
    {
        QMutexLocker lk(&m_mutex);
        return new ThreadsafeNode(*this);
    }

    //Protect model when it fills.
    void insertRows(int row, const QList<BasicItem *> &items, bool notify = true) override
    {
        QMutexLocker lk(&m_mutex);
        BasicItem::insertRows(row, items, notify);
    }

    //Protect model when it remove items.
    void removeRows(int row, int count) override
    {
        QMutexLocker lk(&m_mutex);
        BasicItem::removeRows(row, count);
    }

    //Protect model when it gets items.
    QList<BasicItem*> children() const override
    {
        QMutexLocker lk(&m_mutex);
        return BasicItem::children();
    }

protected:
    //For clone operation only.
    ThreadsafeNode(const ThreadsafeNode &other)
        : BasicItem(other)
    {

    }

    ThreadsafeNode& operator=(const ThreadsafeNode &other) = delete;

private:
    mutable QMutex m_mutex;
};

#endif // THREADSAFENODE_H
