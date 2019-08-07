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

class DownloadManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit DownloadManager(QObject *parent = nullptr)
        : QObject(parent), m_netManager(new QNetworkAccessManager(this))
    {

    }

    explicit DownloadManager(QNetworkAccessManager *netManager, QObject *parent = nullptr)
        : QObject(parent), m_netManager(netManager)
    {

    }

//    explicit DownloadManager(QAbstractItemModel *model, QObject *parent = nullptr)
//        : QObject(parent), m_model(model), m_netManager(new QNetworkAccessManager(this))
//    {

//    }

//    DownloadManager(QAbstractItemModel *model, QNetworkAccessManager *netManager,
//                    QObject *parent = nullptr)
//        : QObject(parent), m_model(model), m_netManager(netManager)
//    {

//    }

    ~DownloadManager() override
    {
        stop();
    }

    void setModel(QAbstractItemModel *model)
    {
        if (m_model != model)
            m_model = model;
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

signals:
    void pathChanged();
    void downloadFinished(const QModelIndex &index);
    void started(QPrivateSignal);
    void stopped(QPrivateSignal);

public slots:
    void start(const QModelIndex &index)
    {
        if (index.isValid())
            startDownload(index);
    }

    void stop(const QModelIndex &index)
    {
        if (index.isValid())
        {
            if (m_model->data(index, VKItemModel::SourceStatusRole).toInt()
                    == VKItem::DownloadingStatus)
                m_downloads.value(index)->abort();
        }
    }

    void pause(const QModelIndex &)
    {
        //Not implemented.
    }

    void start()
    {
        auto tmp = m_model->index(0, 0);
        while (tmp.isValid())
        {
            startDownload(tmp);
            tmp = tmp.siblingAtRow(tmp.row() + 1);
        }
    }

    void stop()
    {
        auto tmp = m_model->index(0, 0);
        while (tmp.isValid())
        {
            stop(tmp);
            tmp = tmp.siblingAtRow(tmp.row() + 1);
        }
    }

    void pause()
    {
        //Not implemented.
    }

private slots:
    void onDownloadReadyRead();
    void onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();

private:
    void startDownload(const QModelIndex &index);

    QAbstractItemModel *m_model = nullptr;
    QNetworkAccessManager *m_netManager;
    QString m_path;
    QMap<QModelIndex, DownloadItem*> m_downloads;
};

#endif // DOWNLOADMANAGER_H
