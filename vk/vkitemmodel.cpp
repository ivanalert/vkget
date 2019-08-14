#include "vkitemmodel.h"
#include "vkitem.h"

#include <QStringBuilder>

VKItemModel::Section VKItemModel::audioReloadSection(const QModelIndex &from)
{
    Section section;
    QVector<Range> ranges;
    bool in = false;
    int counter = 0;

    auto pos = from;
    while (pos.isValid() && counter < 10)
    {
        const auto i = pos.row();
        if (pos.data(SourceRole).toUrl().isEmpty()
                && pos.data(SourceStatusRole).toInt() != VKItem::UnavailableStatus)
        {
            const auto hashes = pos.data(AudioHashRole).toStringList();
            if (hashes.empty())
            {
                section.ids.append(pos.data(IdRole).toString()
                                   % "_" % pos.data(ContentIdRole).toString());
            }
            else
            {
                section.ids.append(pos.data(IdRole).toString()
                                   % "_" % pos.data(ContentIdRole).toString()
                                   % "_" % hashes.at(2)
                                   % "_" % hashes.at(5));
            }

            if (in && section.ranges.last().second.row() + 1 == i)
                section.ranges.last().second = pos;
            else
            {
                //const auto topLeft = index(section.second.last().first, 0);
                //const auto bottomRight = index(section.second.last().second, 0);
                //const QVector<int> roles{SourceStatusRole};
                //emit dataChanged(topLeft, bottomRight, roles);
                section.ranges.append(Range(pos, pos));
            }

            in = true;
            if (section.ids.size() == 10)
                return section;
        }

        pos = pos.siblingAtRow(i + 1);
        ++counter;
    }

    return section;
}
