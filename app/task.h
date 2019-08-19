#ifndef TASK_H
#define TASK_H

#include <QExplicitlySharedDataPointer>

class QFutureWatcherBase;
class TaskData;

class Task
{

public:
    Task();
    Task(QFutureWatcherBase *watcher);
    Task(const Task &other);
    Task& operator=(const Task &other);
    Task(Task &&other) = delete;
    Task& operator=(Task &&other) = delete;
    ~Task();

    bool operator==(const Task &other);

    QFutureWatcherBase* watcher() const noexcept;
    std::atomic<bool>* flag() const noexcept;

private:
    QExplicitlySharedDataPointer<TaskData> m_data;
};

Q_DECLARE_TYPEINFO(Task, Q_MOVABLE_TYPE);
#endif // TASK_H
