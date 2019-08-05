#ifndef BASICITEMMODEL_H
#define BASICITEMMODEL_H

#include "basicitem.h"

#include <QAbstractItemModel>

class BasicItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BasicItemModel(const QVector<QMap<int, QVariant>> &headersData,
                            QObject *parent = nullptr);
    explicit BasicItemModel(int columns, QObject *parent = nullptr);
    explicit BasicItemModel(BasicItem *root, QObject *parent = nullptr);
    ~BasicItemModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        BasicItem *parentItem = getItem(parent);
        return parentItem->rowCount();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        BasicItem *parentItem = getItem(parent);
        return parentItem->columnCount();
    }

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override
    {
        BasicItem *parentItem = getItem(parent);
        return parentItem->hasChildItems();
    }

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeData(const QModelIndex &index, int role);

    QHash<int, QByteArray> roleNames() const override
    {
        auto names = QAbstractItemModel::roleNames();
        names[BasicItem::FlagsRole] = "flags";
        return names;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                       int role) override;

    bool setFlags(const QModelIndex &index, Qt::ItemFlags flags)
    {
        return setData(index, QVariant::fromValue(flags), BasicItem::FlagsRole);
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool canFetchMore(const QModelIndex &parent) const override
    {
        BasicItem *parentItem = getItem(parent);
        return parentItem->populatedChildItemCount() < parentItem->childItemCount();
    }

    void fetchMore(const QModelIndex &parent) override;
    QModelIndex parent(const QModelIndex &index) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex populatedIndex(int row, int column,
                               const QModelIndex &parent = QModelIndex()) const
    {
        if (row < rowCount(parent))
            return index(row, column, parent);
        else
            return QModelIndex();
    }

    QModelIndex lastIndex(int column, const QModelIndex &parent = QModelIndex()) const
    {
        BasicItem *parentItem = getItem(parent);
        return index(parentItem->childItemCount() - 1, column, parent);
    }

    QModelIndex firstIndex(int column, const QModelIndex &parent = QModelIndex()) const
    {
        if (m_rootItem->hasChildItems())
            return index(0, column, parent);

        return QModelIndex();
    }

    template<typename T = BasicItem>
    T* lastItem(const QModelIndex &parent = QModelIndex()) const
    {
        return itemFromIndex<T>(lastIndex(0, parent));
    }

    template<typename T = BasicItem>
    T* firstItem(const QModelIndex &parent = QModelIndex()) const
    {
        return itemFromIndex<T>(firstIndex(0, parent));
    }

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void appendRow(BasicItem *item, const QModelIndex &parent = QModelIndex())
    {
        BasicItem *parentItem = getItem(parent);
        parentItem->appendRow(item);
    }

    void appendRow(const QModelIndex &parent = QModelIndex())
    {
        BasicItem *parentItem = getItem(parent);
        parentItem->appendRow(m_prototype->clone());
    }

    bool insertColumns(int column, int count, const QModelIndex &parent) override;
    bool removeColumns(int column, int count, const QModelIndex &parent) override;

    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value, int hits,
                          Qt::MatchFlags flags) const override;

    template<typename T = BasicItem>
    T* itemFromIndex(const QModelIndex &index) const
    {
        BasicItem *result = getItem(index);
        if (result != m_rootItem)
            return  static_cast<T*>(result);
        else
            return static_cast<T*>(nullptr);
    }

    template<typename T = BasicItem>
    T* itemFromRow(int row, const QModelIndex &parent = QModelIndex()) const
    {
        return itemFromIndex<T>(index(row, 0, parent));
    }

    QModelIndex indexFromItem(BasicItem *item, int column = 0) const;
    QList<QModelIndex> siblingRange(const QModelIndex &first, int count, bool inverse = false);

    template<typename T = BasicItem>
    T* rootItem() const
    {
        return static_cast<T*>(m_rootItem);
    }

    void setItemPrototype(BasicItem *prototype)
    {
        if (m_prototype != prototype)
        {
            delete m_prototype;
            m_prototype = prototype;
        }
    }

    const BasicItem* itemPrototype() const
    {
        return m_prototype;
    }

    int fetchLimit() const noexcept
    {
        return m_fetchLimit;
    }

    void setFetchLimit(int value)
    {
        if (value > 0)
            m_fetchLimit = value;
    }

    template <typename T> QList<T> findItems(const QModelIndex &start, int role,
                                             const QVariant &value, int hits)
    {
        QList<T> result;
        QModelIndex tmpIndex = start;
        while (tmpIndex.isValid() && hits != result.size())
        {
            if (tmpIndex.data(role) == value)
                result.append(static_cast<T>(itemFromIndex(tmpIndex)));
            else
                break;

            tmpIndex = sibling(tmpIndex.row() + 1, tmpIndex.column(), tmpIndex);
        }

        return result;
    }

protected:
    BasicItem* getItem(const QModelIndex &index) const
    {
        if (index.isValid() && index.model() == this)
            return static_cast<BasicItem*>(index.internalPointer());
        else
            return m_rootItem;
    }

private:
    friend class BasicItem;

    BasicItem *m_rootItem;
    BasicItem *m_prototype;
    QVector<QMap<int, QVariant>> m_horizontalHeaderData;
    int m_fetchLimit;

signals:
    void itemChanged(BasicItem *item, int column = -1, const QVector<int> &roles = QVector<int>());

public slots:
    void clear();
};

#endif // BASICITEMMODEL_H
