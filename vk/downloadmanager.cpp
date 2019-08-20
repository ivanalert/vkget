#include "downloadmanager.h"

#include <QDir>
#include <QStringBuilder>

void DownloadManager::startDownload(const QModelIndex &index)
{
    QDir dir(m_path);
    if (dir.exists())
    {
        auto file = new QFile(dir.absoluteFilePath(m_model->data(index).toString()), this);
        if (file->exists())
        {
            if (!m_overwrite)
            {
                m_model->setData(index, 1, VKItemModel::DownloadProgressRole);
                m_model->setData(index, QObject::tr("File already exists"),
                                 VKItemModel::ErrorRole);
                m_model->setData(index, VKItem::ErrorStatus, VKItemModel::SourceStatusRole);

                delete file;
                return;
            }
            else
                file->rename(file->fileName() % ".part");
        }
        else
            file->setFileName(file->fileName() % ".part");

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

            m_downloads.insert(index, download);
            emit downloadCountChanged();
            m_model->setData(index, QString(), VKItemModel::ErrorRole);
            m_model->setData(index, VKItem::DownloadingStatus, VKItemModel::SourceStatusRole);
        }
        else
        {
            m_model->setData(index, file->errorString(), VKItemModel::ErrorRole);
            m_model->setData(index, VKItem::ErrorStatus, VKItemModel::SourceStatusRole);
            delete file;
        }
    }
}

void DownloadManager::start(const QModelIndex &index)
{
    if (index.isValid() && !m_downloads.contains(index))
    {
        if (m_downloads.size() < m_downloadLimit)
        {
            const auto status = m_model->data(index, VKItemModel::SourceStatusRole).toInt();
            if (status == VKItem::ReadyStatus || status == VKItem::ErrorStatus)
            {
                const auto pos = compareTakePendingIndex(index);
                startDownload(pos);
            }
            else
            {
                insertPendingIndex(index);
                if (status == VKItem::NoStatus)
                    emit urlMissing(index);
            }
        }
        else
            insertPendingIndex(index);
    }
}

void DownloadManager::start()
{
    auto pos = m_model->index(0, 0);
    while (pos.isValid())
    {
        if (pos.data(VKItemModel::SourceStatusRole).toInt() != VKItem::UnavailableStatus)
            start(pos);
        pos = pos.siblingAtRow(pos.row() + 1);
    }
}

void DownloadManager::insertPendingIndex(const QModelIndex &index)
{
    auto it = std::lower_bound(m_pendingIndexes.begin(), m_pendingIndexes.end(), index);
    if (it == m_pendingIndexes.end() && !m_pendingIndexes.empty()
            && m_pendingIndexes.last().row() + 1 == index.row())
        m_pendingIndexes.last() = index;
    else if (it.key() != index)
        m_pendingIndexes.insert(index, index);
}

void DownloadManager::removePendingIndex(const QModelIndex &index)
{
    auto it = std::lower_bound(m_pendingIndexes.begin(), m_pendingIndexes.end(), index);
    if (it != m_pendingIndexes.end() && index.row() <= it.value().row())
    {
        const auto start = it.key();
        const auto finish = it.value();
        m_pendingIndexes.erase(it);

        if (index.row() > start.row())
        {
            auto pos = index.siblingAtRow(index.row() - 1);
            m_pendingIndexes.insert(start, pos);
            pos = index.siblingAtRow(index.row() + 1);
            if (pos.isValid())
                m_pendingIndexes.insert(pos, finish);
        }
        else
        {
            auto pos = index.siblingAtRow(index.row() + 1);
            if (pos.isValid() && pos.row() <= finish.row())
                m_pendingIndexes.insert(pos, finish);
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
        m_model->setData(index, VKItem::ReadyStatus, VKItemModel::SourceStatusRole);
    }
    else
    {
        file->remove();
        m_model->setData(index, 0, VKItemModel::DownloadProgressRole);
        m_model->setData(index, res->errorString(), VKItemModel::ErrorRole);
        m_model->setData(index, VKItem::ErrorStatus, VKItemModel::SourceStatusRole);
    }

    delete download;

    auto pos = tryTakePendingIndex();
    while (pos.isValid())
    {
        if (pos.data(VKItemModel::SourceStatusRole).toInt() != VKItem::UnavailableStatus)
        {
            start(pos);
            break;
        }

        pos = tryTakePendingIndex();
    }
}
