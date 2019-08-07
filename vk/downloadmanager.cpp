#include "downloadmanager.h"

#include <QDir>
#include <QStringBuilder>

void DownloadManager::startDownload(const QModelIndex &index)
{
    const auto item = m_model->itemFromIndex<VKItem>(index);
    if (item->sourceStatus() == VKItem::ReadyStatus)
    {
        QDir dir(m_path);
        if (dir.exists())
        {
            auto file = new QFile(dir.absoluteFilePath(item->text()));
            if (file->open(QIODevice::WriteOnly))
            {
                auto res = m_netManager->get(QNetworkRequest(item->source()));
                auto download = new DownloadItem(file, res, index, this);
                connect(download, &DownloadItem::readyRead, this,
                        &DownloadManager::onDownloadReadyRead);
                connect(download, &DownloadItem::downloadProgress, this,
                        &DownloadManager::onDownloadProgressChanged);
                connect(download, &DownloadItem::finished, this,
                        &DownloadManager::onDownloadFinished);
                connect(this, &DownloadManager::stopped, download, &DownloadItem::abort);
                m_downloads.insert(index, download);
            }
            else
                delete file;
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

    if (res->error() == QNetworkReply::NoError)
        file->close();
    else
        file->remove();

    delete download;
}
