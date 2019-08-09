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
        int i = tmp.row();
        if (data(tmp, SourceRole).toUrl().isEmpty()
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
            {
                //const auto topLeft = index(section.second.last().first, 0);
                //const auto bottomRight = index(section.second.last().second, 0);
                //const QVector<int> roles{SourceStatusRole};
                //emit dataChanged(topLeft, bottomRight, roles);
                section.second.append(VKResponse::Range(i, i));
            }

            in = true;
            if (section.first.size() == 10)
                return section;
        }

        tmp = tmp.siblingAtRow(i + 1);
        ++counter;
    }

    return section;
}
