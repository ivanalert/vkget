#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QModelIndex>

class Playlist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QModelIndex previousIndex READ previousIndex NOTIFY previousChanged)
    Q_PROPERTY(QModelIndex currentIndex READ currentIndex NOTIFY currentChanged)
    Q_PROPERTY(QModelIndex nextIndex READ nextIndex NOTIFY nextChanged)
    Q_PROPERTY(int previous READ previous NOTIFY previousChanged)
    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(int next READ next NOTIFY nextChanged)

public:
    Playlist(QAbstractItemModel *model, QObject *parent = nullptr)
        : QObject(parent), m_model(model)
    {
        connect(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), SLOT(invalidate()));
        connect(m_model, &QAbstractItemModel::rowsRemoved, this, &Playlist::onModelRowsRemoved);
        connect(m_model, &QAbstractItemModel::rowsMoved, this, &Playlist::onModelRowsMoved);
        connect(m_model, &QAbstractItemModel::modelReset, this, &Playlist::onModelReset);
        connect(m_model, &QAbstractItemModel::dataChanged, this, &Playlist::onModelDataChanged);
    }

    ~Playlist() override = default;

    void setCurrentIndex(const QModelIndex &index)
    {
        if (m_pos != index)
        {
            m_prevPos = m_pos;
            updatePositions(index);
        }
    }

    void setCurrent(int i)
    {
        setCurrentIndex(m_model->index(i, 0));
    }

    QModelIndex currentIndex() const
    {
        return m_pos;
    }

    int current() const
    {
        return m_pos.row();
    }

    QModelIndex previousIndex() const
    {
        return m_prev;
    }

    int previous() const
    {
        return m_prev.row();
    }

    QModelIndex nextIndex() const
    {
        return m_next;
    }

    int next() const
    {
        return m_next.row();
    }

signals:
    void previousChanged();
    void currentChanged();
    void nextChanged();

public slots:
    void moveBackward()
    {
        setCurrentIndex(previousIndex());
    }

    void moveForward()
    {
        setCurrentIndex(nextIndex());
    }

    void invalidate()
    {
        updatePositions(m_pos);
    }

private slots:
    void onModelRowsInserted(const QModelIndex &parent, int first, int last);
    void onModelRowsRemoved(const QModelIndex &parent, int first, int last);
    void onModelRowsMoved(const QModelIndex &parent, int start, int end,
                          const QModelIndex &destination, int row);
    void onModelReset();
    void onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                            const QVector<int> &roles);

private:
    QModelIndex updatePosition(const QModelIndex &from, int step);
    void updatePreviousPosition(const QModelIndex &from);
    void updateNextPosition(const QModelIndex &from);
    void updatePositions(const QModelIndex &from);

    QAbstractItemModel *m_model;
    QModelIndex m_prev;
    QPersistentModelIndex m_pos;
    QModelIndex m_next;
    QModelIndex m_prevPos;
};

#endif // PLAYLIST_H
