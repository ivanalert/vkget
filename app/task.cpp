#include "task.h"

#include <QSharedData>

class TaskData : public QSharedData
{

public:
    TaskData()
        : watcher(nullptr), flag(false)
    {

    }

    TaskData(QFutureWatcherBase *watcherArg)
        : watcher(watcherArg), flag(false)
    {

    }

    TaskData(const TaskData &other)
        : QSharedData(other)
    {

    }

    ~TaskData() = default;

    QFutureWatcherBase *watcher;
    std::atomic<bool> flag;
};

Task::Task()
    : m_data(new TaskData(nullptr))
{

}

Task::Task(QFutureWatcherBase *watcher)
    : m_data(new TaskData(watcher))
{

}

Task::Task(const Task &other)
    : m_data(other.m_data)
{

}

Task &Task::operator=(const Task &other)
{
    m_data = other.m_data;
    return *this;
}

Task::~Task()
{

}

bool Task::operator==(const Task &other)
{
    return m_data->watcher == other.m_data->watcher;
}

QFutureWatcherBase *Task::watcher() const noexcept
{
    return m_data->watcher;
}

std::atomic<bool> *Task::flag() const noexcept
{
    return &m_data->flag;
}
