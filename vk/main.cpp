#include "vkontakte.h"
#include "vkresponse.h"
#if defined(WEBKIT) || defined(CHROMIUM)
#include "vklogindialog.h"
#include <QApplication>
#elif defined(NATIVE_QML) || defined(CHROMIUM_QML)
#include "vklogindialogqml.h"
#include <QGuiApplication>
#endif
#include <QNetworkCookieJar>
#include <QTextCodec>
#include <memory>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if defined(WEBKIT) || defined(CHROMIUM)
    QApplication app(argc, argv);
    auto dialog = std::make_unique<VKLoginDialog>(
                QLatin1Literal("5571207"), QLatin1Literal("5.95"),
                QStringList{QStringLiteral("audio"), QStringLiteral("video"),
                            QStringLiteral("offline")});
#elif defined(NATIVE_QML) || defined(CHROMIUM_QML)
    QGuiApplication app(argc, argv);
    auto dialog = std::make_unique<VKLoginDialogQml>(
                QLatin1Literal("5571207"), QLatin1Literal("5.95"),
                QStringList{QStringLiteral("audio"), QStringLiteral("video"),
                            QStringLiteral("offline")});
#endif

    if (dialog->exec())
    {
        qDebug() << "logged in successfully";
        qDebug() << "api:" << dialog->version();
        qDebug() << "token:" << dialog->accessToken();
        qDebug() << "user id:" << dialog->userId();
    }
    else
    {
        qDebug() << "failed to log in";
        qDebug() << "error:" << dialog->error();
        return 0;
    }

    const QString userId = dialog->userId();
    VKontakte vk(dialog->version(), dialog->accessToken());
    auto cookieJar = vk.networkAccessManager()->cookieJar();
    for (const auto &cookie : dialog->cookies())
        cookieJar->insertCookie(cookie);

    //Clear dialog resources.
    dialog.reset(nullptr);

    using Param = std::pair<QString, QVariant>;
    auto response = vk.apiRequest(QStringLiteral("users.get"),
                                  QVariantMap{Param(QStringLiteral("user_ids"), userId)});
    QObject::connect(response, &VKResponse::finished,
                     [response = std::unique_ptr<VKResponse>(response)]
                     () mutable
    {
        const auto array = response->parse().object().value(QLatin1Literal("response")).toArray();
        response.reset(nullptr);
        const auto obj = array.at(0).toObject();
        qDebug() << obj.value(QLatin1Literal("can_access_closed")).toBool();
        qDebug() << obj.value(QLatin1Literal("first_name")).toString();
        qDebug() << obj.value(QLatin1Literal("id")).toInt();
        qDebug() << obj.value(QLatin1Literal("is_closed")).toBool();
        qDebug() << obj.value(QLatin1Literal("last_name")).toString();
    });

    QVariantMap params{Param(QStringLiteral("owner_id"), QStringLiteral("88986886")),
                Param(QStringLiteral("act"), QStringLiteral("load_section")),
                Param(QStringLiteral("al"), QStringLiteral("1")),
                Param(QStringLiteral("playlist_id"), QStringLiteral("-1")),
                Param(QStringLiteral("type"), QStringLiteral("playlist")),
                Param(QStringLiteral("offset"), QStringLiteral("0"))};
    response = vk.request(QStringLiteral("al_audio.php"), params);
    QObject::connect(response, &VKResponse::finished,
                     [response = std::unique_ptr<VKResponse>(response)]
                     () mutable
    {
        QByteArray data = std::move(*response).data();
        response.reset(nullptr);
        data.remove(0, data.indexOf('{'));
        int last = data.indexOf("}<");
        data.remove(last + 1, data.size() - last - 1);

        QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
        QString encoded = codec->toUnicode(data);
        data.clear();

        QJsonObject obj = QJsonDocument::fromJson(encoded.toUtf8()).object();
        qDebug() << obj;

        qApp->quit();
    });

    return app.exec();
}
