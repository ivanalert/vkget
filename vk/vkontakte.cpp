#include "vkontakte.h"
#include "vkresponse.h"

#include <QNetworkReply>
#include <QStringBuilder>

QNetworkReply *VKontakte::apiRequest(const QString &method, QUrlQuery &query)
{
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("api.vk.com"));
    url.setPath(QStringLiteral("/method/") % method);

    query.addQueryItem(QStringLiteral("access_token"), accessToken());
    query.addQueryItem(QStringLiteral("v"), apiVersion());

    url.setQuery(query);

    return m_netManager->get(QNetworkRequest(url));
}

QNetworkReply* VKontakte::request(const QString &path, const QUrlQuery &params,
                                  VKontakte::RequestType req)
{
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("vk.com"));
    url.setPath(QStringLiteral("/") % path);

    if (req == VKontakte::RequestType::POST)
    {
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          QLatin1Literal("application/x-www-form-urlencoded;charset=utf-8"));
        return m_netManager->post(request, params.toString().toUtf8());
    }
    else
    {
        url.setQuery(params);
        return m_netManager->get(QNetworkRequest(url));
    }
}

QUrlQuery VKontakte::buildRequestQuery(const QVariantMap &params)
{
    QUrlQuery result;
    for (auto it = params.cbegin(), end = params.cend(); it != end; ++it)
    {
        const auto &val = it.value();
        if (val.type() == QVariant::StringList)
            result.addQueryItem(it.key(), val.toStringList().join(','));
        else
            result.addQueryItem(it.key(), it.value().toString());
    }

    return result;
}
