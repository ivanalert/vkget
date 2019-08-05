#ifndef VKITEMMODEL_H
#define VKITEMMODEL_H

#include "basicitemmodel.h"
#include "vkresponse.h"

class VKItemModel : public BasicItemModel
{
    Q_OBJECT

public:
    enum VKItemDataRole
    {
        IdRole = BasicItem::FlagsRole + 1,
        ContentIdRole,
        SourceRole,
        MetadataRole,
        AudioHashRole,
        SourceStatusRole,
        CoverRole,
        ThumbnailRole
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
        return names;
    }

    VKResponse::Section audioReloadSection(const QModelIndex &start);
};

#endif // VKITEMMODEL_H
