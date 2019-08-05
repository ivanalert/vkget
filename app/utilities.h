#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

namespace Utilities
{

QString removeForbiddenFileNameCharacters(QString name);
QString normalizeHtml(QString html);
QString randomString(int maxChars);
QString secondsToString(int seconds);
QString bytesToString(qint64 bytes);
void findFilePathsRecursive(const QString &path, const QString &pattern, QStringList &result);
void findIconPathsRecursive(const QStringList &themeSearchPaths, const QString &themeName,
                            const QString &iconName, QStringList &result);

}

#endif // UTILITIES_H
