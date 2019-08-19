#include "taskgroup.h"

void TaskGroup::abort(QFutureWatcherBase *watcher)
{
    const auto it = constFind(watcher);
    if (it != m_tasks.cend())
    {
        it->watcher()->cancel();
        it->flag()->store(true);
        it->watcher()->waitForFinished();
    }
}
