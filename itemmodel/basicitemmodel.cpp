#include "basicitemmodel.h"
#include "basicitem.h"

BasicItemModel::BasicItemModel(const QVector<QMap<int, QVariant>> &headersData, QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(new BasicItem(headersData.size())),
      m_prototype(new BasicItem(headersData.size())), m_horizontalHeaderData(headersData),
      m_fetchLimit(100)
{
    m_rootItem->m_model = this;
}

BasicItemModel::BasicItemModel(int columns, QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(new BasicItem(columns)),
      m_prototype(new BasicItem(columns)), m_horizontalHeaderData(columns), m_fetchLimit(100)
{
    m_rootItem->m_model = this;
}

BasicItemModel::BasicItemModel(BasicItem *root, QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(root),
      m_prototype(new BasicItem(root->columnCount())), m_horizontalHeaderData(root->columnCount()),
      m_fetchLimit(100)
{
    m_rootItem->m_model = this;
}

BasicItemModel::~BasicItemModel()
{
    delete m_rootItem;
    delete m_prototype;
}

QVariant BasicItemModel::data(const QModelIndex &index, int role) const
{   
    if (!index.isValid())
        return QVariant();

    BasicItem *item = getItem(index);
    if (item != m_rootItem)
        return item->data(index.column(), role);
    else
        return QVariant();
}

bool BasicItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        BasicItem *item = getItem(index);
        if (item != m_rootItem)
        {
            item->setData(index.column(), value, role, false);
            emit dataChanged(index, index, QVector<int>(1, role));
            return true;
        }
    }

    return false;
}

bool BasicItemModel::removeData(const QModelIndex &index, int role)
{
    if (index.isValid())
    {
        BasicItem *item = getItem(index);
        if (item != m_rootItem)
        {
            item->removeData(index.column(), role, false);
            emit dataChanged(index, index, QVector<int>(1, role));
            return true;
        }
    }

    return false;
}

void BasicItemModel::setData(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                             const QVariant &value, int role)
{
    if (topLeft.isValid() && bottomRight.isValid())
    {
        auto cursor = topLeft;
        for (int i = topLeft.row(), rows = bottomRight.row(); i <= rows; ++i)
        {
            for (int j = topLeft.column(), columns = bottomRight.column(); j <= columns;)
            {
                setData(cursor, value, role);
                cursor = cursor.siblingAtColumn(++j);
            }

            cursor = cursor.siblingAtRow(++i);
        }
    }
}

QVariant BasicItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= 0)
    {
        if (orientation == Qt::Horizontal && section < columnCount())
            return m_horizontalHeaderData.at(section).value(role);
    }

    return QVariant();
}

bool BasicItemModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                                   int role)
{
    if (section >= 0)
    {
        if (orientation == Qt::Horizontal && section < columnCount())
        {
            m_horizontalHeaderData[section].insert(role, value);
            emit headerDataChanged(orientation, section, section);
            return true;
        }
    }

    return false;
}

Qt::ItemFlags BasicItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    BasicItem *item = getItem(index);
    if (item != m_rootItem)
        return item->flags(index.column());
    else
        return Qt::NoItemFlags;
}

void BasicItemModel::fetchMore(const QModelIndex &parent)
{
    BasicItem *parentItem = getItem(parent);
    int first = parentItem->populatedChildItemCount();
    int fetchCount = qMin(m_fetchLimit, parentItem->childItemCount() - first);

    beginInsertRows(parent, first, first + fetchCount - 1);
    parentItem->setPopulatedItemCount(parentItem->populatedChildItemCount() + fetchCount);
    endInsertRows();
}

QModelIndex BasicItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    BasicItem *childItem = getItem(index);
    BasicItem *parentItem = childItem->parentItem();
    if (!parentItem || parentItem == m_rootItem)
        return QModelIndex();
    else
        return createIndex(parentItem->row(), 0, parentItem);
}

QModelIndex BasicItemModel::index(int row, int column, const QModelIndex &parent) const
{
    BasicItem *parentItem = getItem(parent);
    BasicItem *childItem = parentItem->childItem(row);
    if (childItem && column >= 0 && column < childItem->columnCount())
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool BasicItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    BasicItem *parentItem = getItem(parent);
    if (row < 0 || row > parentItem->rowCount())
        return false;

    parentItem->insertRows(row, count, m_prototype);
    return true;
}

bool BasicItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    BasicItem *parentItem = getItem(parent);
    if (row < 0 || row + count > parentItem->rowCount())
        return false;

    parentItem->removeRows(row, count);
    return true;
}

bool BasicItemModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    BasicItem *parentItem = getItem(parent);
    if (column < 0 || column > parentItem->columnCount())
        return false;

    parentItem->insertColumns(column, count);
    return true;
}

bool BasicItemModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    BasicItem *parentItem = getItem(parent);
    if (column < 0 || column + count >= parentItem->columnCount())
        return false;

    parentItem->removeColumns(column, count);
    return true;
}

QModelIndexList BasicItemModel::match(const QModelIndex &start, int role, const QVariant &value,
                                      int hits, Qt::MatchFlags) const
{   
    QModelIndexList result;
    QModelIndex tmpIndex = start;
    while (tmpIndex.isValid() && hits != result.size())
    {
        if (tmpIndex.data(role) == value)
            result.append(tmpIndex);
        else
            break;

        tmpIndex = sibling(tmpIndex.row() + 1, tmpIndex.column(), tmpIndex);
    }

    return result;
}

QModelIndex BasicItemModel::indexFromItem(BasicItem *item, int column) const
{
    if (item == m_rootItem || item->model() != this)
        return QModelIndex();

    QModelIndex parentIndex;
    BasicItem *parentItem = item->parentItem();
    if (parentItem != m_rootItem)
        parentIndex = createIndex(parentItem->row(), 0, parentItem);

    return index(item->row(), column, parentIndex);
}

QList<QModelIndex> BasicItemModel::siblingRange(const QModelIndex &first, int count, bool inverse)
{
    int step = inverse ? -1 : 1;
    QList<QModelIndex> result;
    QModelIndex tmpIndex = first;
    while (tmpIndex.isValid() && count != result.size())
    {
        result.append(tmpIndex);
        tmpIndex = sibling(tmpIndex.row() + step, tmpIndex.column(), tmpIndex);
    }

    return result;
}

void BasicItemModel::clear()
{
    beginResetModel();
    BasicItem *tmp = m_rootItem->clone();
    delete m_rootItem;
    m_rootItem = tmp;
    tmp->m_model = this;
    endResetModel();
}
