#include "downloadmanager.h"

#include <QDir>
#include <QStringBuilder>

void DownloadManager::startDownload(const QModelIndex &index)
{
    if (m_model->data(index, VKItemModel::SourceStatusRole).toInt() == VKItem::ReadyStatus)
    {
        QDir dir(m_path);
        if (dir.exists())
        {
            const QString name = dir.absoluteFilePath(m_model->data(index).toString()) % ".part";
            if (!m_overwrite && QFile::exists(name))
            {
                m_model->setData(index, 1, VKItemModel::DownloadProgressRole);
            }
            else
            {
                auto file = new QFile(name);
                if (file->open(QIODevice::WriteOnly))
                {
                    const auto url = m_model->data(index, VKItemModel::SourceRole).toUrl();
                    auto res = m_netManager->get(QNetworkRequest(url));
                    auto download = new DownloadItem(file, res, index, this);
                    connect(download, &DownloadItem::readyRead, this,
                            &DownloadManager::onDownloadReadyRead);
                    connect(download, &DownloadItem::downloadProgress, this,
                            &DownloadManager::onDownloadProgressChanged);
                    connect(download, &DownloadItem::finished, this,
                            &DownloadManager::onDownloadFinished);
                    connect(this, &DownloadManager::stopped, download, &DownloadItem::abort);

                    m_downloads.insert(index, download);
                    emit downloadCountChanged();
                    m_model->setData(index, VKItem::DownloadingStatus,
                                     VKItemModel::SourceStatusRole);
                }
                else
                    delete file;
            }
        }
    }
}

void DownloadManager::onDownloadReadyRead()
{
    auto download = static_cast<DownloadItem*>(sender());
    auto file = download->file();
    if (file->write(download->reply()->readAll()) == -1)
        download->abort();
}

void DownloadManager::onDownloadProgressChanged(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0)
    {   
        double total = bytesTotal;
        double received = bytesReceived;
        auto download = static_cast<DownloadItem*>(sender());
        m_model->setData(download->index(), received / total, VKItemModel::DownloadProgressRole);
    }
}

void DownloadManager::onDownloadFinished()
{
    auto download = static_cast<DownloadItem*>(sender());
    auto res = download->reply();
    auto file = download->file();
    res->deleteLater();

    const auto index = download->index();
    m_downloads.remove(index);
    emit downloadCountChanged();

    if (res->error() == QNetworkReply::NoError)
    {
        QFileInfo info(*file);
        auto dir = info.dir();
        file->rename(dir.absoluteFilePath(info.completeBaseName()));
        file->close();
        m_model->setData(index, VKItem::ReadyStatus, VKItemModel::SourceStatusRole);
    }
    else
    {
        file->remove();
        m_model->setData(index, 0, VKItemModel::DownloadProgressRole);
        m_model->setData(index, VKItem::ReadyStatus, VKItemModel::SourceStatusRole);
    }

    delete download;
}
