#include "vklogindialog.h"

#include <QUrl>
#include <QUrlQuery>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <memory>

#ifdef CHROMIUM
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#elif WEBKIT
#include <QWebPage>
#include <QNetworkCookieJar>
#endif

VKLoginDialog::VKLoginDialog(const QLatin1String &clientId, const QLatin1String &version,
                             const QStringList &scope, QWidget *parent)
    : QDialog(parent), m_clientId(clientId), m_version(version), m_scope(scope)
{
    std::unique_ptr<QVBoxLayout> vbxLayout(new QVBoxLayout);
    vbxLayout->setContentsMargins(0, 0, 0, 0);

#ifdef CHROMIUM
    QWebEngineProfile::defaultProfile()->cookieStore()->deleteAllCookies();
    std::unique_ptr<QWebEngineView> webEngineView(new QWebEngineView);

    connect(webEngineView.get(), &QWebEngineView::urlChanged, this,
            &VKLoginDialog::onWebViewUrlChanged);
    connect(QWebEngineProfile::defaultProfile()->cookieStore(),
            &QWebEngineCookieStore::cookieAdded, this, &VKLoginDialog::cookieAdded);

    vbxLayout->addWidget(webEngineView.release());
#elif WEBKIT
    std::unique_ptr<QWebView> webView(new QWebView);
    connect(webView.get(), &QWebView::urlChanged, this, &VKLoginDialog::onWebViewUrlChanged);
    vbxLayout->addWidget(webView.release());
#endif

    setLayout(vbxLayout.release());
}

#ifdef WEBKIT
QList<QNetworkCookie> VKLoginDialog::cookies() const
{
    auto webView = static_cast<QWebView*>(layout()->itemAt(0)->widget());
    auto cookieJar = webView->page()->networkAccessManager()->cookieJar();

    QList<QNetworkCookie> cookies;
    cookies.append(cookieJar->cookiesForUrl(QUrl(QStringLiteral("https://www.vk.com/"))));
    cookies.append(cookieJar->cookiesForUrl(QUrl(QStringLiteral("https://www.login.vk.com/"))));

    return cookies;
}
#endif

int VKLoginDialog::exec()
{
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("oauth.vk.com"));
    url.setPath(QStringLiteral("/authorize"));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("client_id"), m_clientId);
    query.addQueryItem(QStringLiteral("redirect_uri"),
                       QStringLiteral("https://oauth.vk.com/blank.html"));
    query.addQueryItem(QStringLiteral("display"), QStringLiteral("popup"));
    query.addQueryItem(QStringLiteral("scope"), m_scope.join(','));
    query.addQueryItem(QStringLiteral("response_type"), QStringLiteral("token"));
    query.addQueryItem(QStringLiteral("revoke"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("v"), m_version);

    url.setQuery(query);

#ifdef CHROMIUM
    auto webEngineview = static_cast<QWebEngineView*>(layout()->itemAt(0)->widget());
    webEngineview->load(url);
#elif WEBKIT
    auto webView = static_cast<QWebView*>(layout()->itemAt(0)->widget());
    webView->load(url);
#endif

    return QDialog::exec();
}

#if defined(CHROMIUM) || defined(WEBKIT)
void VKLoginDialog::onWebViewUrlChanged(const QUrl &url)
{
    if (url.toString(QUrl::RemoveFragment) == "https://oauth.vk.com/blank.html")
    {
        m_fragment.setQuery(url.fragment());
        if (m_fragment.queryItemValue(QStringLiteral("access_token")).isNull())
        {
            if (!m_fragment.isEmpty())
                m_error = m_fragment.queryItemValue(QStringLiteral("error_description"));
            else
                m_error = "unknown error";

            reject();
        }
        else
            accept();
    }
}
#endif

#ifdef CHROMIUM
void VKLoginDialog::cookieAdded(const QNetworkCookie &cookie)
{
    m_cookies.append(cookie);
}
#endif
