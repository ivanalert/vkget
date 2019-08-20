#ifndef VKITEMMODEL_H
#define VKITEMMODEL_H

#include "basicitemmodel.h"

class VKItemModel : public BasicItemModel
{
    Q_OBJECT

public:
    enum VKItemDataRole
    {
        IdRole = BasicItem::FlagsRole + 1, //258
        ContentIdRole, //259
        SourceRole, //260
        MetadataRole, //261
        AudioHashRole, //262
        SourceStatusRole, //263
        CoverRole, //264
        ThumbnailRole, //265
        DownloadProgressRole, //266
        ErrorRole //267
    };

    explicit VKItemModel(QObject *parent = nullptr)
        : BasicItemModel(1, parent)
    {

    }

    explicit VKItemModel(BasicItem *root, QObject *parent = nullptr)
        : BasicItemModel(root, parent)
    {

    }

    ~VKItemModel() override = default;

    // QAbstractItemModel interface
    QHash<int, QByteArray> roleNames() const override
    {
        auto names = BasicItemModel::roleNames();
        names[IdRole] = "id";
        names[ContentIdRole] = "contentId";
        names[SourceRole] = "source";
        names[MetadataRole] = "metadata";
        names[AudioHashRole] = "audioHash";
        names[SourceStatusRole] = "sourceStatus";
        names[CoverRole] = "cover";
        names[ThumbnailRole] = "thumbnail";
        names[DownloadProgressRole] = "downloadProgress";
        names[ErrorRole] = "error";
        return names;
    }

    using Range = QPair<QModelIndex, QModelIndex>;
    struct Section
    {
        QStringList ids;
        QVector<Range> ranges;
    };
    static Section audioReloadSection(const QModelIndex &from);
};

#endif // VKITEMMODEL_H
