#include "vkitemmodel.h"
#include "vkitem.h"

#include <QStringBuilder>

VKResponse::Section VKItemModel::audioReloadSection(const QModelIndex &start)
{
    VKResponse::Section section;
    QVector<VKResponse::Range> ranges;
    bool in = false;
    int counter = 0;

    auto tmp = start;
    while (tmp.isValid() && counter < 10)
    {
        //Need to test.
        /*if (data(tmp, SourceRole).toUrl().isEmpty()
                && data(tmp, SourceStatusRole).toInt() != VKItem::UnavailableStatus)
        {
            setData(tmp, VKItem::LoadingStatus, SourceStatusRole);
            const auto hashes = data(tmp, AudioHashRole).toStringList();
            if (hashes.empty())
            {
                section.first.append(data(tmp, IdRole).toString()
                                     % "_" % data(tmp, ContentIdRole).toString());
            }
            else
            {
                section.first.append(data(tmp, IdRole).toString()
                                     % "_" % data(tmp, ContentIdRole).toString()
                                     % "_" % hashes.at(2)
                                     % "_" % hashes.at(5));
            }

            if (in && section.second.last().second + 1 == i)
                section.second.last().second = i;
            else
                section.second.append(VKResponse::Range(i, i));

            in = true;
            if (section.first.size() == 10)
                return section;
        }*/

        int i = tmp.row();
        const auto item = itemFromIndex<VKItem>(tmp);
        if (item->source().isEmpty() && item->sourceStatus() != VKItem::UnavailableStatus)
        {
            item->setSourceStatus(VKItem::LoadingStatus);

            const auto hashes = item->audioHash();
            if (hashes.empty())
            {
                section.first.append(QString::number(item->id())
                                     % "_" % QString::number(item->contentId()));
            }
            else
            {
                section.first.append(QString::number(item->id())
                                     % "_" % QString::number(item->contentId())
                                     % "_" % hashes.at(2)
                                     % "_" % hashes.at(5));
            }

            if (in && section.second.last().second + 1 == i)
                section.second.last().second = i;
            else
                section.second.append(VKResponse::Range(i, i));

            in = true;
            if (section.first.size() == 10)
                return section;
        }

        tmp = tmp.siblingAtRow(i + 1);
        ++counter;
    }

    return section;
}
