#ifndef VKLOGINDIALOG_H
#define VKLOGINDIALOG_H

#include <QDialog>
#include <QUrlQuery>
#include <QNetworkCookie>

#ifdef CHROMIUM
#include <QWebEngineView>
#elif WEBKIT
#include <QWebView>
#endif

class VKLoginDialog : public QDialog
{
    Q_OBJECT

public:
    VKLoginDialog(const QLatin1String &clientId, const QLatin1String &version,
                  const QStringList &scope, QWidget *parent = nullptr);

    ~VKLoginDialog() override = default;

    QUrlQuery fragment() const noexcept
    {
        return m_fragment;
    }

    QString accessToken() const
    {
        return m_fragment.queryItemValue(QStringLiteral("access_token"));
    }

    QString userId() const
    {
        return m_fragment.queryItemValue(QStringLiteral("user_id"));
    }

    QString expiresIn() const
    {
        return m_fragment.queryItemValue(QStringLiteral("expires_in"));
    }

    QLatin1String clientId() const noexcept
    {
        return m_clientId;
    }

    QLatin1String version() const noexcept
    {
        return m_version;
    }

    QStringList scope() const noexcept
    {
        return m_scope;
    }

    bool isError() const noexcept
    {
        return !m_error.isNull();
    }

    QString error() const noexcept
    {
        return m_error;
    }

#ifdef CHROMIUM
    QList<QNetworkCookie> cookies() const noexcept
    {
        return m_cookies;
    }
#elif WEBKIT
    QList<QNetworkCookie> cookies() const;
#endif

private:
#ifdef CHROMIUM
    QList<QNetworkCookie> m_cookies;
#endif

    const QLatin1String m_clientId;
    const QLatin1String m_version;
    const QStringList m_scope;
    QUrlQuery m_fragment;
    QString m_error;

public slots:
    int exec() override;

private slots:
#if defined(CHROMIUM) || defined(WEBKIT)
    void onWebViewUrlChanged(const QUrl &url);
#endif
#ifdef CHROMIUM
    void cookieAdded(const QNetworkCookie &cookie);
#endif
};

#endif // VKLOGINDIALOG_H
