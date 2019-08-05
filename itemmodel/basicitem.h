#ifndef BASICITEM_H
#define BASICITEM_H

#include <QVariant>
#include <QColor>
#include <QIcon>
#include <QFont>
#include <QBrush>

class BasicItemModel;

class BasicItem
{
public:
    enum ItemDataRole
    {
        //In derived classes expand from this "FlagsRole + 1".
        FlagsRole = Qt::UserRole + 1
    };

    explicit BasicItem(const QVector<QMap<int, QVariant>> &data, BasicItem *parent = nullptr);
    explicit BasicItem(int columns = 1, BasicItem *parent = nullptr);
    BasicItem(BasicItem &&other) noexcept;
    BasicItem& operator=(BasicItem &&other) noexcept;
    virtual ~BasicItem();

    virtual BasicItem* clone() const
    {
        return new BasicItem(*this);
    }

    virtual int columnCount() const
    {
        return m_itemData.size();
    }

    virtual int rowCount() const
    {
        return populatedChildItemCount();
    }

    virtual void setData(int column, const QVariant &value, int role, bool notify = true);
    void removeData(int column, int role, bool notify = true);

    virtual QVariant data(int column, int role) const
    {
        return m_itemData.value(column).value(role);
    }

    void setItemData(int column, const QMap<int, QVariant> &data, bool notify = true);
    QMap<int, QVariant> itemData(int column);

    void setText(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::DisplayRole, notify);
    }

    QString text(int column = 0) const
    {
        return data(column, Qt::DisplayRole).toString();
    }

    void setColor(const QColor &color, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(color), Qt::DecorationRole, notify);
    }

    QColor color(int column = 0) const
    {
        return data(column, Qt::DecorationRole).value<QColor>();
    }

    void setIcon(const QIcon &icon, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(icon), Qt::DecorationRole, notify);
    }

    QIcon icon(int column = 0) const
    {
        return data(column, Qt::DecorationRole).value<QIcon>();
    }

    void setEditText(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::EditRole, notify);
    }

    QString editText(int column = 0) const
    {
        return data(column, Qt::EditRole).toString();
    }

    void setToolTip(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::ToolTipRole, notify);
    }

    QString toolTip(int column = 0) const
    {
        return data(column, Qt::ToolTipRole).toString();
    }

    void setStatusTip(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::StatusTipRole, notify);
    }

    QString statusTip(int column = 0) const
    {
        return data(column, Qt::StatusTipRole).toString();
    }

    void setWhatsThis(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::WhatsThisRole, notify);
    }

    QString whatsThis(int column = 0) const
    {
        return data(column, Qt::WhatsThisRole).toString();
    }

    void setSizeHint(const QSize &size, int column = 0, bool notify = true)
    {
        setData(column, size, Qt::SizeHintRole, notify);
    }

    QSize sizeHint(int column = 0) const
    {
        return data(column, Qt::SizeHintRole).toSize();
    }

    void setFont(const QFont &font, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(font), Qt::FontRole, notify);
    }

    QFont font(int column = 0) const
    {
        return data(column, Qt::FontRole).value<QFont>();
    }

    //Не работает.
    void setTextAlignment(Qt::Alignment alignment, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(alignment), Qt::TextAlignmentRole, notify);
    }

    Qt::Alignment textAlignment(int column = 0) const
    {
        return data(column, Qt::TextAlignmentRole).value<Qt::Alignment>();
    }

    void setBackground(const QBrush &brush, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(brush), Qt::BackgroundRole, notify);
    }

    QBrush background(int column = 0) const
    {
        return data(column, Qt::BackgroundRole).value<QBrush>();
    }

    void setForeground(const QBrush &brush, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(brush), Qt::ForegroundRole, notify);
    }

    QBrush foreground(int column = 0) const
    {
        return data(column, Qt::ForegroundRole).value<QBrush>();
    }

    void setCheckState(Qt::CheckState state, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(state), Qt::CheckStateRole, notify);
    }

    Qt::CheckState checkState(int column = 0) const
    {
        return data(column, Qt::CheckStateRole).value<Qt::CheckState>();
    }

    void setInitialSortOrder(Qt::SortOrder order, int column = 0, bool notify = true)
    {
        setData(column, QVariant::fromValue(order), Qt::InitialSortOrderRole, notify);
    }

    Qt::SortOrder initialSortOrder(int column = 0) const
    {
        return data(column, Qt::InitialSortOrderRole).value<Qt::SortOrder>();
    }

    void setAccessibleText(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::AccessibleTextRole, notify);
    }

    QString accessibleText(int column = 0) const
    {
        return data(column, Qt::AccessibleTextRole).toString();
    }

    void setAccessibleDescription(const QString &text, int column = 0, bool notify = true)
    {
        setData(column, text, Qt::AccessibleDescriptionRole, notify);
    }

    QString accessibleDescription(int column = 0) const
    {
        return data(column, Qt::AccessibleDescriptionRole).toString();
    }

    void setUserData(const QVariant &var, int column = 0, bool notify = true)
    {
        setData(column, var, Qt::UserRole, notify);
    }

    QVariant userData(int column = 0) const
    {
        return data(column, Qt::UserRole);
    }

    void setFlags(Qt::ItemFlags flags, int column = 0, bool notify = true);

    void setFlags(Qt::ItemFlags flags, const QVector<int> &columns, bool notify = true)
    {
        for (int column : columns)
            setFlags(flags, column, notify);
    }

    Qt::ItemFlags flags(int column = 0) const;
    void setEnabled(bool notify = true);
    void setDisabled(bool notify = true);

    int populatedChildItemCount() const
    {
        return m_populatedChildItemCount;
    }

    bool hasParentItem() const
    {
        return m_parentItem;
    }

    BasicItem* parentItem() const
    {
        return m_parentItem;
    }

    virtual QList<BasicItem*> children() const
    {
        return m_childItems;
    }

    bool hasChildItems() const
    {
        return !children().empty();
    }

    BasicItem* childItem(int row) const
    {
        return children().value(row, nullptr);
    }

    int childItemCount() const
    {
        return children().size();
    }

    BasicItemModel* model() const
    {
        return m_model;
    }

    QModelIndex modelIndex() const;

    int row() const
    {
        return m_row;
    }

    virtual void insertRows(int row, const QList<BasicItem*> &items);
    virtual void removeRows(int row, int count);
    virtual void insertColumns(int column, int count);
    virtual void removeColumns(int column, int count);

    void insertRows(int row, int count, BasicItem *prototype = nullptr);

    void insertRow(int row, BasicItem *item)
    {
        insertRows(row, QList<BasicItem*>({item}));
    }

    void insertRow(int row)
    {
        insertRows(row, 1);
    }

    void appendRow(BasicItem *item)
    {
        insertRows(childItemCount(), QList<BasicItem*>{item});
    }

    void appendRow()
    {
        insertRows(childItemCount(), 1);
    }

protected:
    BasicItem(const BasicItem &other);
    BasicItem& operator=(const BasicItem &other);

    void setPopulatedItemCount(int value)
    {
        m_populatedChildItemCount = value;
    }

    void emitDataChanged(int firstColumn, int lastColumn,
                         const QVector<int> &roles = QVector<int>());

private:
    friend class BasicItemModel;

    void updateChildItems(int first);

    void updateChildRows(int first)
    {
        for (int end = m_childItems.size(); first < end; ++first)
            m_childItems[first]->m_row = first;
    }

    void updateChildModels(int first)
    {
        for (int end = m_childItems.size(); first < end; ++first)
        {
            BasicItem *childItem = m_childItems.at(first);
            childItem->m_model = m_model;
            if (childItem->hasChildItems())
                childItem->updateChildModels(0);
        }
    }

    //Data.
    QVector<QMap<int, QVariant>> m_itemData;

    //Hierarchy.
    BasicItem *m_parentItem;
    QList<BasicItem*> m_childItems;
    int m_populatedChildItemCount;

    //Position.
    BasicItemModel *m_model;
    int m_row;
};

#endif // BASICITEM_H
