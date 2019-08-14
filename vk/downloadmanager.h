#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "vkitemmodel.h"
#include "vkitem.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QModelIndex>

//Helper class.
class DownloadItem : public QObject
{
    Q_OBJECT

public:
    DownloadItem(QFile *file, QNetworkReply *reply, const QModelIndex &index,
                 QObject *parent = nullptr)
        : QObject(parent), m_file(file), m_reply(reply), m_index(index)
    {
        file->setParent(this);
        connect(m_reply, &QNetworkReply::readyRead, this, &DownloadItem::readyRead);
        connect(m_reply, &QNetworkReply::downloadProgress, this, &DownloadItem::downloadProgress);
        connect(m_reply, &QNetworkReply::finished, this, &DownloadItem::finished);
    }

    ~DownloadItem() override = default;

    QNetworkReply* reply() const
    {
        return m_reply;
    }

    QFile* file() const
    {
        return m_file;
    }

    QModelIndex index() const
    {
        return m_index;
    }

signals:
    void readyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();

public slots:
    void abort()
    {
        m_reply->abort();
    }

private:
    QFile *m_file;
    QNetworkReply *m_reply;
    QModelIndex m_index;
};

//Need to send it to worker thread for async i/o.
class DownloadManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool overwrite READ overwrite WRITE setOverwrite NOTIFY overwriteChanged)
    Q_PROPERTY(int downloadCount READ downloadCount NOTIFY downloadCountChanged)

public:
    explicit DownloadManager(QAbstractItemModel *model, QObject *parent = nullptr)
        : QObject(parent), m_model(model), m_netManager(new QNetworkAccessManager(this))
    {
        //Stop all downloads before model will be destroyed.
        connect(m_model, SIGNAL(destroyed(QObject*)), SLOT(stop()));
    }

    explicit DownloadManager(QAbstractItemModel *model, QNetworkAccessManager *netManager,
                             QObject *parent = nullptr)
        : QObject(parent), m_model(model), m_netManager(netManager)
    {
        //Stop all downloads before model will be destroyed.
        connect(m_model, SIGNAL(destroyed(QObject*)), SLOT(stop()));
    }

    ~DownloadManager() override
    {
        stop();
    }

    QAbstractItemModel* model() const noexcept
    {
        return m_model;
    }

    void setPath(const QString &value)
    {
        if (m_path != value)
        {
            m_path = value;
            emit pathChanged();
        }
    }

    QString path() const
    {
        return m_path;
    }

    void setOverwrite(bool value)
    {
        if (m_overwrite != value)
        {
            m_overwrite = value;
            emit overwriteChanged();
        }
    }

    bool overwrite() const noexcept
    {
        return m_overwrite;
    }

    int downloadCount() const
    {
        return m_downloads.size();
    }

signals:
    void pathChanged();
    void overwriteChanged();
    void downloadCountChanged();
    void urlMissing(const QModelIndex &index);

public slots:
    void start(const QModelIndex &index);

    void start()
    {
        startFrom(m_model->index(0, 0));
    }

    void startFrom(const QModelIndex &index);

    void stop(const QModelIndex &index)
    {
        auto download = m_downloads.value(index, nullptr);
        if (download)
            download->abort();
    }

    void stop()
    {
        //To prevent enque new downloads.
        m_pendingIndexes.clear();
        while (!m_downloads.empty())
            m_downloads.first()->abort();
    }

    void pause(const QModelIndex &)
    {
        //Not implemented.
    }

    void pause()
    {
        //Not implemented.
    }

    bool isPendingIndex(const QModelIndex &index)
    {
        const auto it = std::lower_bound(m_pendingIndexes.cbegin(), m_pendingIndexes.cend(),
                                         index);
        return  it != m_pendingIndexes.cend() && index.row() <= it.value().row();
    }

private slots:
    void onDownloadReadyRead();
    void onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();

private:
    void startDownload(const QModelIndex &index);
    void insertPendingIndex(const QModelIndex &index);
    void removePendingIndex(const QModelIndex &index);

    QModelIndex compareTakePendingIndex(const QModelIndex &index)
    {
        if (!m_pendingIndexes.empty())
        {
            const auto res = m_pendingIndexes.firstKey();
            if (res == index)
            {
                removePendingIndex(res);
                return res;
            }
        }

        return  QModelIndex();
    }

    QModelIndex tryTakePendingIndex()
    {
        if (!m_pendingIndexes.empty())
            return takePendingIndex();
        return QModelIndex();
    }

    QModelIndex takePendingIndex()
    {
        const auto res = m_pendingIndexes.firstKey();
        removePendingIndex(res);
        return res;
    }

    QAbstractItemModel *m_model = nullptr;
    QNetworkAccessManager *m_netManager;
    QString m_path;
    QMap<QModelIndex, DownloadItem*> m_downloads;
    bool m_overwrite = false;
    QMap<QModelIndex, QModelIndex> m_pendingIndexes;

    static constexpr int m_downloadLimit = 5;
};

#endif // DOWNLOADMANAGER_H
