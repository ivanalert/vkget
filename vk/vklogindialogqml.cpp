#include "vklogindialogqml.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <memory>
//Can't get all cookies with QtWebView. Need to use cookie storage on disk.
#ifdef NATIVE_QML
#include <QtWebView>
#else
#include <QtWebEngine>
#endif

void VKLoginDialogQml::open()
{
#ifdef NATIVE_QML
    QtWebView::initialize();
    const QByteArray qml("import QtQuick 2.12\n"
                         "import QtQuick.Window 2.12\n"
                         "import QtWebView 1.1\n"
                         "Window {\n"
                         "id: root\n"
                         "objectName: \"dialogWindow\"\n"
                         "title: qsTr(\"VKontakte\")\n"
                         "modality: Qt.ApplicationModal\n"
                         "flags: Qt.Dialog\n"
                         "visible: true\n"
                         "width: 650\n"
                         "height: 500\n"
                         "WebView {\n"
                         "id: webView\n"
                         "objectName: \"webView\"\n"
                         "anchors.fill: parent\n"
                         "}\n"
                         "}\n");
#else
    QtWebEngine::initialize();
    QQuickWebEngineProfile::defaultProfile()->cookieStore()->deleteAllCookies();
    connect(QQuickWebEngineProfile::defaultProfile()->cookieStore(),
            &QWebEngineCookieStore::cookieAdded, this, &VKLoginDialogQml::onCookieAdded);
    const QByteArray qml("import QtQuick 2.12\n"
                         "import QtQuick.Window 2.12\n"
                         "import QtWebEngine 1.8\n"
                         "Window {\n"
                         "id: root\n"
                         "objectName: \"root\"\n"
                         "title: qsTr(\"VKontakte\")\n"
                         "modality: Qt.ApplicationModal\n"
                         "flags: Qt.Dialog\n"
                         "visible: true\n"
                         "width: 650\n"
                         "height: 500\n"
                         "WebEngineView {\n"
                         "id: webView\n"
                         "objectName: \"webView\"\n"
                         "anchors.fill: parent\n"
                         "}\n"
                         "}\n");
#endif

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

    bool engine = m_engine;
    if (!engine)
        m_engine = new QQmlEngine(this);

    auto comp = std::make_unique<QQmlComponent>(m_engine);
    comp->setData(qml, QUrl());
    m_root = comp->create();
    connect(m_root, SIGNAL(closing(QQuickCloseEvent*)), SLOT(onClosing(QQuickCloseEvent*)));

    if (engine)
        //Alternative ownership (QQmlEngine::JavaScriptOwnership).
        QQmlEngine::setObjectOwnership(m_root, QQmlEngine::JavaScriptOwnership);
    else
        //Default ownership (QQmlEngine::CppOwnership).
        m_root->setParent(m_engine);

    auto webView = m_root->findChild<QObject*>(QStringLiteral("webView"));
    connect(webView, SIGNAL(urlChanged()), SLOT(onUrlChanged()));
    webView->setProperty("url", url);
}

int VKLoginDialogQml::exec()
{
    QEventLoop *wait = new QEventLoop(this);
    connect(this, &VKLoginDialogQml::accepted, wait, &QEventLoop::quit);
    connect(this, &VKLoginDialogQml::rejected, wait, &QEventLoop::quit);
    open();
    wait->exec();

    return m_result;
}

void VKLoginDialogQml::onUrlChanged()
{
    const auto url = sender()->property("url").toUrl();
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
