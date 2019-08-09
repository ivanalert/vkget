#include "playlist.h"
#include "vkitem.h"
#include "vkitemmodel.h"

void Playlist::onModelRowsInserted(const QModelIndex &/*parent*/, int /*first*/, int /*last*/)
{
    invalidate();
}

void Playlist::onModelRowsRemoved(const QModelIndex &/*parent*/, int /*first*/, int /*last*/)
{
    invalidate();
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
            if (m_model->data(m_model->index(i, 0), VKItemModel::SourceStatusRole).toInt()
                    == VKItem::UnavailableStatus && start >= previous()
                    && (next() < 0 || finish <= next()))
                invalidate();
    }
}

QModelIndex Playlist::updatePosition(const QModelIndex &from, int step)
{
    auto tmp = from;
    while (tmp.isValid())
    {
        if (m_model->data(tmp, VKItemModel::SourceStatusRole).toInt()
                != VKItem::UnavailableStatus)
            return tmp;

        tmp = tmp.siblingAtRow(tmp.row() + step);
    }

    return tmp;
}

void Playlist::updatePositions(const QModelIndex &from)
{
    auto tmp = updatePosition(from, 1);
    if (m_pos != tmp)
    {
        m_pos = tmp;
        emit currentChanged();
    }

    tmp = updatePosition(m_pos.sibling(m_pos.row() - 1, 0), -1);
    if (m_prev != tmp)
    {
        m_prev = tmp;
        emit previousChanged();
    }

    tmp = updatePosition(m_pos.sibling(m_pos.row() + 1, 0), 1);
    if (m_next != tmp)
    {
        m_next = tmp;
        emit nextChanged();
    }
}
