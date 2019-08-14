#include "utilities.h"

#include <QRegularExpression>
#include <QStringBuilder>
#include <QDir>
#include <random>

QString Utilities::removeForbiddenFileNameCharacters(QString name)
{
    name.remove('\\');
    name.remove('/');
    name.remove(':');
    name.remove('*');
    name.remove('?');
    name.remove('\"');
    name.remove('<');
    name.remove('>');
    name.remove('|');
    name.remove('+');
    name.remove('%');
    name.remove('!');
    name.remove('@');

    return name.simplified();
}

QString Utilities::normalizeHtml(QString html)
{
    QRegularExpression rx(QStringLiteral("&#(\\d+)?;"));
    QRegularExpressionMatch match = rx.match(html);
    while (match.hasMatch())
    {
        html.replace(match.capturedStart(), match.capturedLength(),
                       QChar(match.captured(1).toInt()));
        match = rx.match(html, match.capturedStart() + 1);
    }
    html.replace("&quot;", "\"");
    html.replace("&amp;", "&");
    html.replace("&lt;", "<");
    html.replace("&gt;", ">");

    return html;
}

QString Utilities::randomString(int maxChars)
{
    const QString chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
    std::random_device rng;
    std::uniform_int_distribution<> index(0, chars.size() - 1);
    QString result;
    while (maxChars)
    {
        result += chars.at(index(rng));
        --maxChars;
    }

    return result;
}

QString Utilities::secondsToString(int seconds)
{
    int s = seconds;
    int m = 0;
    int h = 0;

    if (s > 60)
    {
        m = s / 60;
        s = s - m * 60;
    }
    if (m > 60)
    {
        h = m / 60;
        m = m - h * 60;
    }

    QString result;
    if (h != 0)
    {
        result += h < 10 ? QString::number(h).prepend('0') : QString::number(h);
        result += ":";
    }
    if (m != 0 || !result.isNull())
    {
        result += m < 10 ? QString::number(m).prepend('0') : QString::number(m);
        result += ":";
    }
    if (s != 0 || !result.isNull())
    {
        result += s < 10 ? QString::number(s).prepend('0') : QString::number(s);
    }

    return result;
}

QString Utilities::bytesToString(qint64 bytes)
{
    double size = bytes;
    double divide = 0;
    for (int i = 0;; ++i)
    {
        divide = size / 1000;
        if (divide < 1 || i == 3)
        {
            QString sizeStr = QString::number(size, 'f', 2);
            switch (i)
            {
            case 0:
                sizeStr += " " + QObject::tr("b");
                break;
            case 1:
                sizeStr += " " + QObject::tr("kb");
                break;
            case 2:
                sizeStr += " " + QObject::tr("mb");
                break;
            case 3:
                sizeStr += " " + QObject::tr("gb");
                break;
            default:
                break;
            }

            return sizeStr;
        }

        size = divide;
    }
}

void Utilities::findFilePathsRecursive(const QString &path, const QString &pattern,
                                       QStringList &result)
{
    QDir currDir(path);
    for (const QString &currPath : currDir.entryList(QStringList(pattern), QDir::Files))
        result.append(path % QDir::separator() % currPath);

    for (const QString &currPath : currDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        findFilePathsRecursive(path % QDir::separator() % currPath, pattern, result);
}

void Utilities::findIconPathsRecursive(const QStringList &themeSearchPaths,
                                       const QString &themeName, const QString &iconName,
                                       QStringList &result)
{
    QDir currDir;
    for (const QString &path : themeSearchPaths)
    {
        currDir.setPath(path);
        QStringList entryList = currDir.entryList(QStringList(themeName),
                                                  QDir::Dirs | QDir::NoDotAndDotDot);
        if (!entryList.empty())
        {
            findFilePathsRecursive(currDir.absolutePath() % QDir::separator()
                                   % entryList.first(), iconName, result);
            break;
        }
    }
}
