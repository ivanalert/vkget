#include "basicitem.h"
#include "basicitemmodel.h"

BasicItem::BasicItem(const QVector<QMap<int, QVariant>> &data, BasicItem *parent)
    : m_itemData(data), m_parentItem(parent), m_populatedChildItemCount(0), m_model(nullptr),
      m_row(-1)
{
    if (m_parentItem)
        m_parentItem->appendRow(this);
}

BasicItem::BasicItem(int columns, BasicItem *parent)
    : m_itemData(columns), m_parentItem(parent), m_populatedChildItemCount(0), m_model(nullptr),
      m_row(-1)
{
    if (m_parentItem)
        m_parentItem->appendRow(this);
}

BasicItem::BasicItem(BasicItem &&other) noexcept
    : m_itemData(std::move(other.m_itemData)), m_parentItem(other.m_parentItem),
      m_childItems(std::move(other.m_childItems)),
      m_populatedChildItemCount(other.m_populatedChildItemCount), m_model(other.m_model),
      m_row(other.m_row)
{
    other.m_parentItem = nullptr;
    other.m_populatedChildItemCount = 0;
    other.m_model = nullptr;
    other.m_row = -1;
}

BasicItem &BasicItem::operator=(BasicItem &&other) noexcept
{
    m_itemData = std::move(other.m_itemData);
    m_parentItem = other.m_parentItem;
    other.m_parentItem = nullptr;
    m_childItems = std::move(other.m_childItems);
    m_populatedChildItemCount = other.m_populatedChildItemCount;
    other.m_populatedChildItemCount = 0;
    m_model = other.m_model;
    other.m_model = nullptr;
    m_row = other.m_row;
    other.m_row = -1;

    return *this;
}

BasicItem::BasicItem(const BasicItem &other)
    : m_itemData(other.m_itemData), m_parentItem(nullptr), m_populatedChildItemCount(0),
      m_model(nullptr), m_row(-1)
{

}

BasicItem &BasicItem::operator=(const BasicItem &other)
{
    m_itemData = other.m_itemData;
    m_parentItem = nullptr;
    m_populatedChildItemCount = 0;
    m_model = nullptr;
    m_row = -1;

    return *this;
}

BasicItem::~BasicItem()
{
    if (hasChildItems())
        removeRows(0, childItemCount());
}

void BasicItem::emitDataChanged(int firstColumn, int lastColumn, const QVector<int> &roles)
{
    if (m_model)
    {
        QModelIndex topLeft = modelIndex().sibling(m_row, firstColumn);
        QModelIndex bottomRight = topLeft.sibling(m_row, lastColumn);
        emit m_model->dataChanged(topLeft, bottomRight, roles);
    }
}

void BasicItem::updateChildItems(int first)
{
    for (int end = m_childItems.size(); first < end; ++first)
    {
        BasicItem *childItem = m_childItems.at(first);
        childItem->m_row = first;
        childItem->m_model = m_model;
        if (childItem->hasChildItems())
            childItem->updateChildModels(0);
    }
}

void BasicItem::setData(int column, const QVariant &value, int role, bool notify)
{
    m_itemData[column].insert(role, value);

    if (notify)
        emitDataChanged(column, column, QVector<int>(1, role));
}

void BasicItem::removeData(int column, int role, bool notify)
{
    m_itemData[column].remove(role);

    if (notify)
        emitDataChanged(column, column, QVector<int>(1, role));
}

void BasicItem::setItemData(int column, const QMap<int, QVariant> &data, bool notify)
{
    m_itemData[column] = data;

    if (notify)
    {
        QVector<int> roles;
        roles.reserve(data.size());
        for (int role : data.keys())
            roles.append(role);

        emitDataChanged(column, column, roles);
    }
}

QMap<int, QVariant> BasicItem::itemData(int column)
{
    return m_itemData.at(column);
}

void BasicItem::setFlags(Qt::ItemFlags flags, int column, bool notify)
{
    setData(column, QVariant::fromValue(flags), FlagsRole, notify);
}

Qt::ItemFlags BasicItem::flags(int column) const
{
    QVariant var = m_itemData.at(column).value(FlagsRole);
    if (var.isValid())
        return var.value<Qt::ItemFlags>();
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void BasicItem::setEnabled(bool notify)
{
    for (int i = 0, size = columnCount(); i < size; ++i)
        removeData(i, FlagsRole, notify);
}

void BasicItem::setDisabled(bool notify)
{
    for (int i = 0, size = columnCount(); i < size; ++i)
        setData(i, Qt::NoItemFlags, FlagsRole, notify);
}

QModelIndex BasicItem::modelIndex() const
{
    if (m_model)
        return m_model->indexFromItem(const_cast<BasicItem*>(this));
    else
        return QModelIndex();
}

void BasicItem::insertRows(int row, const QList<BasicItem*> &items, bool notify)
{
    int pos = row;
    if (m_model && notify)
    {
        m_model->beginInsertRows(m_model->indexFromItem(this), row, row + items.size() - 1);
        for (BasicItem *item : items)
        {
            item->m_parentItem = this;
            item->m_model = m_model;
            m_childItems.insert(pos++, item);
        }

        m_populatedChildItemCount += items.size();
        updateChildItems(row);
        m_model->endInsertRows();
    }
    else
    {
        for (BasicItem *item : items)
        {
            item->m_parentItem = this;
            item->m_model = m_model;
            m_childItems.insert(pos++, item);
        }

        updateChildItems(row);
    }
}

void BasicItem::insertRows(int row, int count, BasicItem *prototype, bool notify)
{
    if (!prototype)
        prototype = this;

    QList<BasicItem*> items;
    items.reserve(count);
    for (int i = 0; i < count; ++i)
        items.append(prototype->clone());

    insertRows(row, items, notify);
}

void BasicItem::removeRows(int row, int count)
{
    if (m_model)
    {
        m_model->beginRemoveRows(m_model->indexFromItem(this), row, row + count - 1);
        for (int i = row + count - 1; i >= row; --i)
            delete m_childItems.takeAt(i);

        int end = row + count - 1;
        if (end < m_populatedChildItemCount)
            m_populatedChildItemCount = m_populatedChildItemCount - count;
        else
            m_populatedChildItemCount = m_populatedChildItemCount
                    - (count - (end - m_populatedChildItemCount + 1));

        updateChildRows(row);
        m_model->endRemoveRows();
    }
    else
    {
        for (int i = row + count - 1; i >= row; --i)
            delete m_childItems.takeAt(i);

        updateChildRows(row);
    }
}

void BasicItem::insertColumns(int column, int count)
{
    if (m_model)
    {
        m_model->beginInsertColumns(m_model->indexFromItem(this), column, column + count - 1);
        for (int i = 0; i < count; ++i)
            m_itemData.insert(column++, QMap<int, QVariant>());
        m_model->endInsertColumns();
    }
    else
    {
        for (int i = 0; i < count; ++i)
            m_itemData.insert(column++, QMap<int, QVariant>());
    }
}

void BasicItem::removeColumns(int column, int count)
{
    if (m_model)
    {
        m_model->beginRemoveColumns(m_model->indexFromItem(this), column, column + count - 1);
        m_itemData.remove(column, count);
        m_itemData.squeeze();
        m_model->endRemoveColumns();
    }
    else
    {
        m_itemData.remove(column, count);
        m_itemData.squeeze();
    }
}
