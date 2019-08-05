#ifndef VKLOGINDIALOGQML_H
#define VKLOGINDIALOGQML_H

#include <QObject>
#include <QUrlQuery>
#include <QNetworkCookie>
#include <QVariant>

class QQmlEngine;
class QQuickCloseEvent;

class VKLoginDialogQml : public QObject
{
    Q_OBJECT

public:
    enum Result {
        Rejected,
        Accepted
    };

    VKLoginDialogQml(QQmlEngine *engine, const QLatin1String &clientId,
                     const QLatin1String &version, const QStringList &scope,
                     QObject *parent = nullptr)
        : QObject(parent), m_engine(engine), m_clientId(clientId), m_version(version),
          m_scope(scope)
    {

    }

    VKLoginDialogQml(const QLatin1String &clientId, const QLatin1String &version,
                     const QStringList &scope, QObject *parent = nullptr)
        : QObject(parent), m_engine(nullptr), m_clientId(clientId), m_version(version),
          m_scope(scope)
    {

    }

    ~VKLoginDialogQml() override = default;

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

    QList<QNetworkCookie> cookies() const noexcept
    {
        return m_cookies;
    }

    int result() const noexcept
    {
        return m_result;
    }

    bool isError() const
    {
        return !m_error.isNull();
    }

    QString error() const noexcept
    {
        return m_error;
    }

signals:
    void accepted();
    void rejected();

public slots:
    void open();
    int exec();

    void accept()
    {
        m_result = Result::Accepted;
        if (m_root)
            QMetaObject::invokeMethod(m_root, "close");
        emit accepted();
    }

    void reject()
    {
        m_result = Result::Rejected;
        if (m_root)
            QMetaObject::invokeMethod(m_root, "close");
        emit rejected();
    }

private slots:
    void onUrlChanged();

    void onClosing(QQuickCloseEvent *)
    {
        if (m_fragment.isEmpty())
            emit rejected();
    }
#if CHROMIUM_QML
    void onCookieAdded(const QNetworkCookie &cookie)
    {
        m_cookies.append(cookie);
    }
#endif

private:
    QQmlEngine *m_engine;
    const QLatin1String m_clientId;
    const QLatin1String m_version;
    const QStringList m_scope;
    QObject *m_root = nullptr;

    QUrlQuery m_fragment;
    QList<QNetworkCookie> m_cookies;
    Result m_result = Result::Rejected;
    QString m_error;
};

#endif // VKLOGINDIALOGQML_H
