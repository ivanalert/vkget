#ifndef VKONTAKTE_H
#define VKONTAKTE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrlQuery>

class VKontakte : public QObject
{
    Q_OBJECT

public:
    enum class RequestType
    {
        GET,
        POST
    };

    VKontakte(const QString &api, const QString &token, QObject *parent = nullptr)
        : QObject(parent), m_apiVersion(api), m_accessToken(token),
          m_netManager(new QNetworkAccessManager(this))
    {

    }

    VKontakte(const QString &api, const QString &token, QNetworkAccessManager *manager,
              QObject *parent = nullptr)
        : QObject(parent), m_apiVersion(api), m_accessToken(token), m_netManager(manager)
    {

    }

    ~VKontakte() override = default;

    bool isLoggedIn() const
    {
        return !m_accessToken.isNull() && !m_apiVersion.isNull();
    }

    void setApiVersion(const QString &value)
    {
        m_apiVersion = value;
    }

    QString apiVersion() const noexcept
    {
        return m_apiVersion;
    }

    void setAccessToken(const QString &value)
    {
        m_accessToken = value;
    }

    QString accessToken() const noexcept
    {
        return m_accessToken;
    }

    bool isValid() const
    {
        return !m_apiVersion.isEmpty() && !m_accessToken.isEmpty();
    }

    QNetworkAccessManager* networkAccessManager() const noexcept
    {
        return m_netManager;
    }

    //Value in map can be QString or QStringList.
    QNetworkReply* apiRequest(const QString &method, const QVariantMap &params)
    {
        QUrlQuery query = buildRequestQuery(params);
        return apiRequest(method, query);
    }

    QNetworkReply* apiRequest(const QString &path, QUrlQuery &params);

    //Value in map can be QString or QStringList.
    QNetworkReply* request(const QString &path, const QVariantMap &params)
    {
        QUrlQuery query = buildRequestQuery(params);
        return request(path, query);
    }

    QNetworkReply* request(const QString &path, const QUrlQuery &params,
                           VKontakte::RequestType req = VKontakte::RequestType::POST);

private:
    QUrlQuery buildRequestQuery(const QVariantMap &params);

    QString m_apiVersion;
    QString m_accessToken;
    QNetworkAccessManager *m_netManager;
};

#endif // VKONTAKTE_H
