#ifndef VKITEM_H
#define VKITEM_H

#include "basicitem.h"
#include "vkitemmodel.h"

#include <QUrl>

class VKItem : public BasicItem
{
public:
    enum SourceStatus
    {
        ReadyStatus,
        LoadingStatus,
        UnavailableStatus,
        NoStatus,
        DownloadingStatus,
        DownloadPausedStatus,
        PendingStatus,
        ErrorStatus
    };

    VKItem(const int id, const QString &name, BasicItem *parent = nullptr)
        : BasicItem(1, parent)
    {
        setId(id);
        setText(name);
    }

    VKItem(VKItem &&other) noexcept
        : BasicItem(std::move(other))
    {

    }

    VKItem& operator=(VKItem &&other) noexcept
    {
        BasicItem::operator=(std::move(other));
        return *this;
    }

    ~VKItem() override = default;

    // BasicItem interface
    BasicItem *clone() const override
    {
        return new VKItem(*this);
    }

    void setId(const int value, bool notify = true)
    {
        setData(0, value, VKItemModel::IdRole, notify);
    }

    int id() const
    {
        return data(0, VKItemModel::IdRole).toInt();
    }

    void setContentId(const int value, bool notify = true)
    {
        setData(0, value, VKItemModel::ContentIdRole, notify);
    }

    int contentId() const
    {
        return data(0, VKItemModel::ContentIdRole).toInt();
    }

    void setSource(const QUrl &value, bool notify = true)
    {
        setData(0, value, VKItemModel::SourceRole, notify);
    }

    QUrl source() const
    {
        return data(0, VKItemModel::SourceRole).toUrl();
    }

    void setMetadata(const QString &value, bool notify = true)
    {
        setData(0, value, VKItemModel::MetadataRole, notify);
    }

    QString metadata() const
    {
        return data(0, VKItemModel::MetadataRole).toString();
    }

    void setAudioHash(const QStringList &value, bool notify = true)
    {
        setData(0, value, VKItemModel::AudioHashRole, notify);
    }

    QStringList audioHash() const
    {
        return data(0, VKItemModel::AudioHashRole).toStringList();
    }

    void setSourceStatus(SourceStatus value, bool notify = true)
    {
        setData(0, value, VKItemModel::SourceStatusRole, notify);
    }

    int sourceStatus() const
    {
        return data(0, VKItemModel::SourceStatusRole).toInt();
    }

    void setCover(const QUrl &value, bool notify = true)
    {
        setData(0, value, VKItemModel::CoverRole, notify);
    }

    QUrl cover() const
    {
        return data(0, VKItemModel::CoverRole).toUrl();
    }

    void setThumbnail(const QUrl &value, bool notify = true)
    {
        setData(0, value, VKItemModel::ThumbnailRole, notify);
    }

    QUrl thumbnail() const
    {
        return data(0, VKItemModel::ThumbnailRole).toUrl();
    }

    void setDownloadProgress(double value, bool notify = true)
    {
        setData(0, value, VKItemModel::DownloadProgressRole, notify);
    }

    double downloadProgress() const
    {
        return data(0, VKItemModel::DownloadProgressRole).toDouble();
    }

    void setError(const QString &value, bool notify = true)
    {
        setData(0, value, VKItemModel::ErrorRole, notify);
    }

    QString error() const
    {
        return data(0, VKItemModel::ErrorRole).toString();
    }

protected:
    VKItem(const VKItem &other)
        : BasicItem(other)
    {

    }

    VKItem& operator=(const VKItem &other)
    {
        BasicItem::operator=(other);
        return *this;
    }
};

#endif // VKITEM_H
