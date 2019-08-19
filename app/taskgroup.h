#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "task.h"

#include <QObject>
#include <QFutureWatcherBase>

class TaskGroup : public QObject
{
    Q_OBJECT

public:
    explicit TaskGroup(QObject *parent = nullptr)
        : QObject(parent)
    {

    }

    TaskGroup(const TaskGroup &other) = delete;
    TaskGroup& operator=(const TaskGroup &other) = delete;
    TaskGroup(TaskGroup &&other) = delete;
    TaskGroup& operator=(TaskGroup &&other) = delete;

    ~TaskGroup() override
    {
        abort();
    }

    const std::atomic<bool>* append(QFutureWatcherBase *watcher)
    {
        m_tasks.append(Task(watcher));
        connect(watcher, &QFutureWatcherBase::finished, this, &TaskGroup::onWatcherFinsihed);
        return  m_tasks.last().flag();
    }

    void abort(QFutureWatcherBase *watcher);

    void abort()
    {
        for (const auto &task : m_tasks)
        {
            task.watcher()->cancel();
            task.flag()->store(true);
            task.watcher()->waitForFinished();
        }
    }

    void raiseFlags()
    {
        for (const auto &task : m_tasks)
            task.flag()->store(true);
    }

    int count() const
    {
        return m_tasks.size();
    }

    bool empty() const
    {
        return m_tasks.empty();
    }

private slots:
    void onWatcherFinsihed()
    {
        remove(static_cast<QFutureWatcherBase*>(sender()));
    }

private:
    QList<Task>::const_iterator constFind(const QFutureWatcherBase *watcher)
    {
        return std::find_if(m_tasks.cbegin(), m_tasks.cend(),
                            [watcher](const Task &other)
        {
            return watcher == other.watcher();
        });
    }

    QList<Task>::iterator find(const QFutureWatcherBase *watcher)
    {
        return std::find_if(m_tasks.begin(), m_tasks.end(),
                            [watcher](const Task &other)
        {
            return watcher == other.watcher();
        });
    }

    void remove(QFutureWatcherBase *watcher)
    {
        auto it = find(watcher);
        if (it != m_tasks.end())
            m_tasks.erase(it);
    }

    QList<Task> m_tasks;
};

#endif // TASKGROUP_H
