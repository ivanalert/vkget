#ifndef VKRESPONSE_H
#define VKRESPONSE_H

#include <QString>
#include <QJsonDocument>
#include <QUrl>
#include <QModelIndex>

namespace VKResponse
{

QJsonValue responseOrError(const QByteArray &data, bool &ok, QJsonParseError *error = nullptr);
QByteArray decodeAudioUrl(int id, const QByteArray &encoded);
QUrl decodeAudioUrl(int id, const QUrl &encoded);
QString decodeAudioUrl(int id, const QString &encoded);
QJsonDocument parseAudioSection(QByteArray &data, const QString &contentTypeHeader,
                                QJsonParseError *error = nullptr);
QJsonDocument parseAudioReloadSection(QByteArray &data, const QString &contentTypeHeader,
                                      QJsonParseError *error = nullptr);

using Range = QPair<int, int>;
using Section = QPair<QStringList, QVector<Range>>;
QVector<Section> audioSectionsForReload(const QJsonArray &list);
QVector<Range> emptyRanges(const QModelIndex &first, int dataRole, int limit = 10);

}

#endif // VKRESPONSE_H
