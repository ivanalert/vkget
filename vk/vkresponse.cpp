#include "vkresponse.h"

#include <algorithm>
#include <QStringBuilder>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QVarLengthArray>
#include <QTextCodec>

const QByteArray ALL("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN0PQRSTUVWXYZO123456789+/=");

QJsonValue VKResponse::responseOrError(const QByteArray &data, bool &ok, QJsonParseError *error)
{
    const auto obj = QJsonDocument::fromJson(data, error).object();
    if (obj.contains("response"))
    {
        ok = true;
        return obj.value("response");
    }
    else
    {
        ok = false;
        return obj.value("error");
    }
}

QByteArray aFunc(const QByteArray &str)
{
    if (str.isEmpty() || str.size() % 4 == 1)
        return QByteArray();

    QByteArray res;
    int i, e, o;
    i = e = o = 0;

    for (int s = 0, size = str.size(); s < size; ++s)
    {
        e = ALL.indexOf(str.at(s));
        if (e != -1)
        {
            if (o % 4)
                i = 64 * i + e;
            else
                i = e;

            if (o++ % 4)
                res += char(255 & i >> (-2 * o & 6));
        }
    }

    return res;
}

QVarLengthArray<int> sFunc(const QByteArray &str, int number)
{
    int size = str.size();
    QVarLengthArray<int> res(size);
    if (!res.empty())
    {
        int i = size;
        while (i--)
        {
            number = (size * (i + 1) ^ number + i) % size;
            res[i] = number;
        }
    }

    return res;
}

void sDecode(QByteArray &str, int number)
{
    if (!str.isEmpty())
    {
        QVarLengthArray<int> o = sFunc(str, number);
        for (int i = 1, size = str.size(); i < size; ++i)
        {
            int index = o.at(size - 1 - i);
            if (index >= size)
                index = size - 1;

            //Swap chars.
            const auto tmp = str.at(i);
            str[i] = str.at(index);
            str[index] = tmp;
        }
    }
}

QByteArray xDecode(const QByteArray &str1, const QByteArray &str2)
{
    QByteArray res;
    char ch = str2.at(0);
    for (int i = 0; i < str1.size(); ++i)
    {
        char currCh(str1.at(i));
        res += currCh ^ ch;
    }

    return res;
}

QByteArray VKResponse::decodeAudioUrl(int id, const QByteArray &encoded)
{
    QByteArray res;
    QList<QByteArray> mask = encoded.split('?');
    if (mask.size() > 1 && !mask[1].remove(mask.at(1).indexOf("extra="), 6).isEmpty())
    {
        mask = mask.at(1).split('#');
        if (mask.size() > 1)
        {
            res = aFunc(mask.at(0));
            mask = aFunc(mask.at(1)).split(9);

            QList<QByteArray> decodeStrs;
            auto it = mask.crbegin();
            auto end = mask.crend();
            while (it != end)
            {
                decodeStrs = it->split(11);
                if (decodeStrs.size() > 1)
                {
                    switch (decodeStrs.at(0).at(0))
                    {
                    case 'i':
                        sDecode(res, decodeStrs.at(1).toInt() ^ id);
                        break;
                    case 'r':
                        std::reverse(res.begin(), res.end());
                        break;
                    case 's':
                        sDecode(res, decodeStrs.at(1).toInt());
                        break;
                    case 'x':
                        res = xDecode(res, decodeStrs.at(1));
                        break;
                    }

                    ++it;
                }
            }
        }
    }

    return res;
}

QUrl VKResponse::decodeAudioUrl(int id, const QUrl &encoded)
{
    return QUrl(decodeAudioUrl(id, encoded.toString().toUtf8()));
}

QString VKResponse::decodeAudioUrl(int id, const QString &encoded)
{
    return decodeAudioUrl(id, encoded.toUtf8());
}

QJsonDocument VKResponse::parseAudioSection(QByteArray &data, const QString &contentTypeHeader,
                                            QJsonParseError *error)
{
    data.remove(0, data.indexOf('{'));
    auto last = data.indexOf("}<");
    data.remove(last + 1, data.size() - last - 1);

    //Try to get charset from reply header.
    QByteArray charset;
    const auto strs = contentTypeHeader.split(';', QString::SkipEmptyParts);
    for (const auto &str : strs)
    {
        if (str.contains("charset"))
        {
            auto subStrs = str.trimmed().split('=', QString::SkipEmptyParts);
            if (subStrs.size() > 1)
                charset.append(subStrs.at(1));
        }
    }

    auto codec = QTextCodec::codecForName(charset);
    if (!codec)
        codec = QTextCodec::codecForName("UTF-8");
    const auto encoded = codec->toUnicode(data);
    data.clear();

    return QJsonDocument::fromJson(encoded.toUtf8(), error);
}

QJsonDocument VKResponse::parseAudioReloadSection(QByteArray &data,
                                                  const QString &contentTypeHeader,
                                                  QJsonParseError *error)
{
    data.remove(0, data.indexOf('['));
    auto last = data.indexOf("]<");
    data.remove(last + 1, data.size() - last - 1);

    //Try to get charset from reply header.
    QByteArray charset;
    const auto strs = contentTypeHeader.split(';', QString::SkipEmptyParts);
    for (const auto &str : strs)
    {
        if (str.contains("charset"))
        {
            auto subStrs = str.trimmed().split('=', QString::SkipEmptyParts);
            if (subStrs.size() > 1)
                charset.append(subStrs.at(1));
        }
    }

    auto codec = QTextCodec::codecForName(charset);
    if (!codec)
        codec = QTextCodec::codecForName("UTF-8");
    const auto encoded = codec->toUnicode(data);
    data.clear();

    return QJsonDocument::fromJson(encoded.toUtf8(), error);
}

QVector<VKResponse::Section> VKResponse::audioSectionsForReload(const QJsonArray &list)
{
    QStringList ids;
    QVector<Range> ranges;
    QVector<Section> sections;
    bool in = false;

    for (int i = 0, size = list.size(); i < size; ++i)
    {
        const auto array = list.at(i).toArray();
        if (array.at(2).toString().isEmpty())
        {
            const auto audioHashes = array.at(13).toString().split('/');
            if (audioHashes.empty())
            {
                ids.append(QString::number(array.at(1).toInt())
                           % "_" % QString::number(array.at(0).toInt()));
            }
            else
            {
                ids.append(QString::number(array.at(1).toInt())
                           % "_" % QString::number(array.at(0).toInt())
                           //actionHash.
                           % "_" % audioHashes.at(2)
                           //urlHash.
                           % "_" % audioHashes.at(5));
            }

            if (in && ranges.last().second + 1 == i)
                ranges.last().second = i;
            else
                ranges.append(Range(i, i));

            in = true;
            if (ids.size() == 10)
            {
                sections.append(Section(ids, ranges));
                ids.clear();
                ranges.clear();
                in = false;
            }
        }
    }

    if (!ids.empty())
        sections.append(Section(ids, ranges));

    return sections;
}

QVector<VKResponse::Range> VKResponse::emptyRanges(const QModelIndex &first, int dataRole,
                                                   int limit)
{
    QVector<Range> ranges;
    bool in = false;
    int counter = 0;

    auto tmp = first;
    while (tmp.isValid())
    {
        int i = tmp.row();
        if (tmp.data(dataRole).toString().isEmpty())
        {
            ++counter;
            if (in && ranges.last().second + 1 == i)
                ranges.last().second = i;
            else
                ranges.append(Range(i, i));

            in = true;
            if (counter == limit)
                return ranges;
        }

        tmp = tmp.siblingAtRow(i + 1);
    }

    return ranges;
}
