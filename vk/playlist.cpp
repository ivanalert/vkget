#include "playlist.h"
#include "vkitem.h"

void Playlist::onModelRowsInserted(const QModelIndex &/*parent*/, int /*first*/, int /*last*/)
{
    //Not implemented.
}

void Playlist::onModelRowsRemoved(const QModelIndex &/*parent*/, int /*first*/, int /*last*/)
{
    //Not implemented.
}

void Playlist::onModelRowsMoved(const QModelIndex &/*parent*/, int /*start*/, int /*end*/,
                                const QModelIndex &/*destination*/, int /*row*/)
{
    //Not implemented.
}

void Playlist::onModelReset()
{
    if (m_prev.isValid())
    {
        m_prev = QModelIndex();
        emit previousChanged();
    }

    if (m_pos.isValid())
    {
        m_pos = QModelIndex();
        emit currentChanged();
    }

    if (m_next.isValid())
    {
        m_next = QModelIndex();
        emit nextChanged();
    }
}

void Playlist::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                  const QVector<int> &roles)
{
    if (roles.indexOf(VKItemModel::SourceStatusRole) != -1)
    {
        const auto start = topLeft.row();
        const auto finish = bottomRight.row();
        for (auto i = start; i <= finish; ++i)
            if (m_model->itemFromRow<VKItem>(i)->sourceStatus() == VKItem::UnavailableStatus
                    && start >= previous() && (next() < 0 || finish <= next()))
                updatePositions(m_pos);
    }
}

void Playlist::updatePositions(const QModelIndex &index)
{
    bool changed = false;
    auto tmp = index;
    while (tmp.isValid())
    {
        const auto item = m_model->itemFromIndex<VKItem>(tmp);
        if (item->sourceStatus() != VKItem::UnavailableStatus)
        {
            m_pos = tmp;
            changed = true;
            emit currentChanged();
            break;
        }

        //populatedIndex is thread safe if model fetchs in the same thread.
        tmp = m_model->populatedIndex(tmp.row() + 1, 0);
    }

    if (!changed && m_pos != tmp)
    {
        m_pos = tmp;
        emit currentChanged();
    }

    if (m_pos.isValid())
    {
        changed = false;
        tmp = m_model->populatedIndex(m_pos.row() - 1, 0);
        while (tmp.isValid())
        {
            const auto item = m_model->itemFromIndex<VKItem>(tmp);
            if (item->sourceStatus() != VKItem::UnavailableStatus)
            {
                m_prev = tmp;
                changed = true;
                emit previousChanged();
                break;
            }

            tmp = m_model->populatedIndex(tmp.row() - 1, 0);
        }

        if (!changed && m_prev != tmp)
        {
            m_prev = tmp;
            emit previousChanged();
        }

        changed = false;
        tmp = m_model->populatedIndex(m_pos.row() + 1, 0);
        while (tmp.isValid())
        {
            const auto item = m_model->itemFromIndex<VKItem>(tmp);
            if (item->sourceStatus() != VKItem::UnavailableStatus)
            {
                m_next = tmp;
                emit nextChanged();
                break;
            }

            //Because model fills in other thread. To prevent race condition.
            tmp = m_model->populatedIndex(tmp.row() + 1, 0);
        }

        if (!changed && m_next != tmp)
        {
            m_next = tmp;
            emit nextChanged();
        }
    }
    else
    {
        if (m_prevPos.isValid())
            //To prevent changing of local index need copy.
            setCurrentIndex(QModelIndex(m_prevPos));
        else
        {
            if (m_prev.isValid())
            {
                m_prev = QModelIndex();
                emit previousChanged();
            }

            if (m_next.isValid())
            {
                m_next = QModelIndex();
                emit nextChanged();
            }
        }
    }
}
