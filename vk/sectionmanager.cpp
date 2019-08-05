#include "sectionmanager.h"
#include "vkontakte.h"

#include <QNetworkReply>

SectionManager::SectionManager(const SectionManager::ManagedSection &sec, VKontakte *vk,
                               QObject *parent)
    : QObject(parent), m_managed(sec), m_vk(vk), m_timer(new QTimer(this)),
      m_request(0), m_reply(sec.second.size()), m_aborted(false)
{
    m_timer->setInterval(67);
    connect(m_timer, &QTimer::timeout, this, &SectionManager::onTimeout);
}

void SectionManager::onTimeout()
{
    const auto ids = m_managed.second.at(m_request).first;
    const auto ranges = m_managed.second.at(m_request).second;

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("act"), QStringLiteral("reload_audio"));
    query.addQueryItem(QStringLiteral("al"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("ids"), ids.join(','));

    int index = m_request;
    auto res = m_vk->request(QStringLiteral("al_audio.php"), query);
    connect(res, &QNetworkReply::finished,
            [this, res, index]
    {
        onReloadSectionFinished(res, index);
    });
    connect(this, &SectionManager::aborted, res, &QNetworkReply::abort);

    //Stop when all section requests have been sended.
    ++m_request;
    if (m_request == m_managed.second.size())
        m_timer->stop();
}

void SectionManager::onReloadSectionFinished(QNetworkReply *reply, int index)
{
    --m_reply;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> res(reply);
    if (res->error() == QNetworkReply::NoError)
    {
        auto &list = m_managed.first;
        auto &sec = m_managed.second[index];

        auto data = res->readAll();
        const auto doc = VKResponse::parseAudioReloadSection(
                    data, res->header(QNetworkRequest::ContentTypeHeader).toString());
        res.reset();

        const auto array = doc.array();
        if (!array.isEmpty())
        {
            int j = 0;
            for (const auto &range : sec.second)
                for (int i = range.first; i <= range.second; ++i, ++j)
                    list[i] = array.at(j);
        }
    }

    //Ready whe all section replies have been received.
    if (!m_reply && !m_aborted)
        emit ready(m_managed.first);
}
