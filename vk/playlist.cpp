#include "playlist.h"
#include "vkitem.h"
#include "vkitemmodel.h"

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
            if (m_model->data(m_model->index(i, 0), VKItemModel::SourceStatusRole).toInt()
                    == VKItem::UnavailableStatus && start >= previous()
                    && (next() < 0 || finish <= next()))
                updatePositions(m_pos);
    }
}

void Playlist::updatePositions(const QModelIndex &index)
{
    bool changed = false;
    auto tmp = index;
    while (tmp.isValid())
    {
        if (m_model->data(tmp, VKItemModel::SourceStatusRole).toInt()
                != VKItem::UnavailableStatus)
        {
            m_pos = tmp;
            changed = true;
            emit currentChanged();
            break;
        }

        tmp = tmp.siblingAtRow(tmp.row() + 1);
    }

    if (!changed && m_pos != tmp)
    {
        m_pos = tmp;
        emit currentChanged();
    }

    if (m_pos.isValid())
    {
        changed = false;
        tmp = m_pos.siblingAtRow(m_pos.row() - 1);
        while (tmp.isValid())
        {
            if (m_model->data(tmp, VKItemModel::SourceStatusRole).toInt()
                    != VKItem::UnavailableStatus)
            {
                m_prev = tmp;
                changed = true;
                emit previousChanged();
                break;
            }

            tmp = tmp.siblingAtRow(tmp.row() - 1);
        }

        if (!changed && m_prev != tmp)
        {
            m_prev = tmp;
            emit previousChanged();
        }

        changed = false;
        tmp = m_pos.siblingAtRow(m_pos.row() + 1);
        while (tmp.isValid())
        {
            if (m_model->data(tmp, VKItemModel::SourceStatusRole).toInt()
                    != VKItem::UnavailableStatus)
            {
                m_next = tmp;
                emit nextChanged();
                break;
            }

            tmp = tmp.siblingAtRow(tmp.row() + 1);
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
