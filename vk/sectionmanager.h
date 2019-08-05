#ifndef SECTIONMANAGER_H
#define SECTIONMANAGER_H

#include "vkresponse.h"

#include <QObject>
#include <QJsonArray>
#include <QTimer>

class VKontakte;
class QNetworkReply;

class SectionManager : public QObject
{
    Q_OBJECT

public:
    using ManagedSection = QPair<QJsonArray, QVector<VKResponse::Section>>;

    SectionManager(const ManagedSection &sec, VKontakte *vk, QObject *parent = nullptr);
    ~SectionManager() override = default;

signals:
    void ready(const QJsonArray &list);
    void aborted(QPrivateSignal);

public slots:
    void start()
    {
        m_request = 0;
        m_reply = m_managed.second.size();
        m_aborted = false;
        m_timer->start();
    }

    void stop()
    {
        m_timer->stop();
    }

    void abort()
    {
        stop();
        m_aborted = true;
        emit aborted(QPrivateSignal());
    }

    bool isAborted() const noexcept
    {
        return m_aborted;
    }

private slots:
    void onTimeout();
    void onReloadSectionFinished(QNetworkReply *reply, int index);

private:
    ManagedSection m_managed;
    VKontakte *m_vk;
    QTimer *m_timer;

    int m_request;
    int m_reply;
    bool m_aborted;
};

#endif // SECTIONMANAGER_H
