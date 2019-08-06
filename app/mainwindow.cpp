#include "mainwindow.h"
#include "vkontakte.h"
#include "vklogindialogqml.h"
#include "vkitemmodel.h"
#include "vkitem.h"
#include "threadsafenode.h"
#include "playlist.h"
#include "utilities.h"

#include <memory>
#include <QtConcurrent>
#include <QGuiApplication>
#include <QClipboard>
#include <QFutureWatcherBase>
#include <QStringBuilder>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QJsonDocument>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSettings>
#include <QDir>

MainWindow::MainWindow(QObject *parent)
    : QObject(parent), m_engine(new QQmlEngine(this)),
      m_friends(new VKItemModel(new ThreadsafeNode, this)),
      m_groups(new VKItemModel(new ThreadsafeNode, this)),
      m_audios(new VKItemModel(new ThreadsafeNode, this)),
      m_playlist(new Playlist(m_audios, this)), m_navLog(new NavigationLog(this))
{
    m_engine->rootContext()->setContextProperty("friends", m_friends);
    m_engine->rootContext()->setContextProperty("groups", m_groups);
    m_engine->rootContext()->setContextProperty("audios", m_audios);
    m_engine->rootContext()->setContextProperty("audioPlaylist", m_playlist);
    m_engine->rootContext()->setContextProperty("navigationLog", m_navLog);

    auto comp = std::make_unique<QQmlComponent>(m_engine, QUrl(QStringLiteral("qrc:/main.qml")));
    m_root = static_cast<QQuickWindow*>(comp->create());

    auto action = m_root->findChild<QObject*>(QStringLiteral("loginAction"));
    connect(action, SIGNAL(triggered()), SLOT(onLoginTriggered()));
    action = m_root->findChild<QObject*>(QStringLiteral("refreshAction"));
    connect(action, SIGNAL(triggered()), SLOT(usersGet()));
    action = m_root->findChild<QObject*>(QStringLiteral("previousAction"));
    connect(action, SIGNAL(triggered()), SLOT(onPreviousTriggered()));
    action = m_root->findChild<QObject*>(QStringLiteral("nextAction"));
    connect(action, SIGNAL(triggered()), SLOT(onNextTriggered()));
    action = m_root->findChild<QObject*>(QStringLiteral("homeAction"));
    connect(action, SIGNAL(triggered()), SLOT(onHomeTriggered()));
    action = m_root->findChild<QObject*>(QStringLiteral("urlToClipboard"));
    connect(action, SIGNAL(triggered()), SLOT(onUrlToClipboardTriggered()));

    m_navigation = m_root->findChild<QObject*>(QStringLiteral("navigation"));

    m_friendsView = m_root->findChild<QQuickItem*>(QStringLiteral("friendsView"));
    connect(m_friendsView, SIGNAL(itemRequested()), SLOT(onFriendsViewItemRequested()));
    m_groupsView = m_root->findChild<QQuickItem*>(QStringLiteral("groupsView"));
    connect(m_groupsView, SIGNAL(itemRequested()), SLOT(onGroupsViewItemRequested()));

    m_audiosView = m_root->findChild<QQuickItem*>(QStringLiteral("audiosView"));
    m_playbackControl = m_root->findChild<QQuickItem*>(QStringLiteral("playbackControl"));
    m_player = m_root->findChild<QObject*>(QStringLiteral("audioPlayer"));
    connect(m_playlist, &Playlist::currentChanged, this, &MainWindow::onPlaylistCurrentChaged);

    //Load settings. Temporary, to prevent login every time app launches.
    QSettings settings(QSettings::UserScope, QStringLiteral("ivanalert@mail.ru"),
                       QStringLiteral("VKGet"));
    settings.beginGroup(QStringLiteral("vk"));
    m_vk = new VKontakte(settings.value(QStringLiteral("api_version")).toString(),
                         settings.value(QStringLiteral("access_token")).toString(), this);
    if (m_vk->isValid())
    {
        auto cookieJar = m_vk->networkAccessManager()->cookieJar();
        auto count = settings.beginReadArray(QStringLiteral("cookie"));
        for (int i = 0; i < count; ++i)
        {
            QNetworkCookie cookie;
            settings.setArrayIndex(i);
            cookie.setName(settings.value(QStringLiteral("name")).toByteArray());
            cookie.setValue(settings.value(QStringLiteral("value")).toByteArray());
            cookie.setDomain(settings.value(QStringLiteral("domain")).toString());
            cookie.setPath(settings.value(QStringLiteral("path")).toString());
            cookie.setExpirationDate(settings.value(QStringLiteral("expires")).toDateTime());
            cookie.setHttpOnly(settings.value(QStringLiteral("httpOnly")).toBool());
            cookie.setSecure(settings.value(QStringLiteral("secure")).toBool());
            cookieJar->insertCookie(cookie);
        }
        settings.endArray();

        usersGet();
    }
    else
    {
        m_friendsView->setProperty("error", "You are no logged in.");
        m_friendsView->setProperty("status", 3);
        m_groupsView->setProperty("error", "You are no logged in.");
        m_groupsView->setProperty("status", 3);
        m_audiosView->setProperty("error", "You are no logged in.");
        m_audiosView->setProperty("status", 3);
    }
    settings.endGroup();
}

MainWindow::~MainWindow()
{
    //Save settings. Temporary, to prevent login every time app launches.
    QSettings settings(QSettings::UserScope, QStringLiteral("ivanalert@mail.ru"),
                       QStringLiteral("VKGet"));
    settings.beginGroup(QStringLiteral("vk"));
    settings.setValue(QStringLiteral("api_version"), m_vk->apiVersion());
    settings.setValue(QStringLiteral("access_token"), m_vk->accessToken());
    settings.beginWriteArray(QStringLiteral("cookie"));

    int i = 0;
    auto cookieJar = m_vk->networkAccessManager()->cookieJar();
    QList<QNetworkCookie> cookies;
    cookies.append(cookieJar->cookiesForUrl(QUrl(QStringLiteral("https://www.vk.com/"))));
    cookies.append(cookieJar->cookiesForUrl(QUrl(QStringLiteral("https://www.login.vk.com/"))));
    for (const auto &cookie : cookies)
    {
        if (!cookie.isSessionCookie())
        {
            settings.setArrayIndex(i++);
            settings.setValue(QStringLiteral("name"), cookie.name());
            settings.setValue(QStringLiteral("value"), cookie.value());
            settings.setValue(QStringLiteral("domain"), cookie.domain());
            settings.setValue(QStringLiteral("path"), cookie.path());
            settings.setValue(QStringLiteral("expires"), cookie.expirationDate());
            settings.setValue(QStringLiteral("httpOnly"), cookie.isHttpOnly());
            settings.setValue(QStringLiteral("secure"), cookie.isSecure());
        }
    }
    settings.endArray();
    settings.endGroup();
}

void MainWindow::abortTasks()
{
    emit aborted();

    for (auto it = m_tasks.begin(), end = m_tasks.end(); it != end;)
    {
        auto watcher = static_cast<QFutureWatcherBase*>(it.key());
        watcher->cancel();
        //Relaxed.
        it.value().store(1);
        watcher->waitForFinished();
        it = m_tasks.erase(it);
    }
}

//static. async.
BasicItem *MainWindow::createUserItem(const QJsonValue &value, const QString &name)
{
    const auto obj = value.toObject();
    auto item = new VKItem(obj.value("id").toInt(), obj.value(name).toString());
    item->setData(0, obj.value("photo_100").toString(), Qt::DecorationRole);
    return item;
}

void MainWindow::usersGet(int id)
{
    //Abort async tasks and net requests if any. Wait all tasks to finish. No need mutex here.
    abortTasks();
    m_friends->clear();
    m_groups->clear();
    m_audios->clear();
    m_firstAudios = true;

    QUrlQuery query;
    if (id != std::numeric_limits<int>::max())
        query.addQueryItem(QStringLiteral("user_ids"), QString::number(id));
    query.addQueryItem(QStringLiteral("fields"), QStringLiteral("photo_200"));

    auto res = m_vk->apiRequest(QStringLiteral("users.get"), query);
    connect(res, &QNetworkReply::finished, this, &MainWindow::onUsersGetFinished);
    connect(this, &MainWindow::aborted, res, &QNetworkReply::abort);
}

void MainWindow::onUsersGetFinished()
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> res(
                static_cast<QNetworkReply*>(sender()));
    if (res->error() == QNetworkReply::NoError)
    {
        bool ok = false;
        const auto val = VKResponse::responseOrError(res.get()->readAll(), ok);
        res.reset();
        if (ok)
        {
            //Enable UI.
            m_root->setProperty("enabled", true);

            const auto user = val.toArray().at(0).toObject();
            QString name = user.value("first_name").toString()
                    % " " % user.value("last_name").toString();
            m_navigation->setProperty("userName", name);
            m_navigation->setProperty("userAvatar", user.value("photo_200").toString());
            m_id = user.value("id").toInt();

            //Navigation history.
            if (m_logHistory)
                logNavigation();

            friendsGet(m_id);
            groupsGet(m_id);
            audiosGet(m_id);
        }
        else
        {
            //Handle error! Disable UI.
            m_friendsView->setProperty("error", val.toObject().value("error_msg").toString());
            m_friendsView->setProperty("status", 3);
            m_groupsView->setProperty("error", val.toObject().value("error_msg").toString());
            m_groupsView->setProperty("status", 3);
            m_audiosView->setProperty("error", val.toObject().value("error_msg").toString());
            m_audiosView->setProperty("status", 3);
        }
    }
}

void MainWindow::onFriendsViewItemRequested()
{
    auto row = sender()->property("currentIndex").toInt();
    //Threadsafe if only fill model.
    const auto id = static_cast<VKItem*>(m_friends->itemFromRow(row))->id();
    m_logHistory = true;
    usersGet(id);
}

void MainWindow::friendsGet(int id, int offset)
{
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("user_id"), QString::number(id));
    query.addQueryItem(QStringLiteral("fields"), QStringLiteral("photo_100"));
    query.addQueryItem(QStringLiteral("offset"), QString::number(offset));

    auto res = m_vk->apiRequest(QStringLiteral("friends.get"), query);
    connect(res, &QNetworkReply::finished, this, &MainWindow::onFriendsGetFinished);
    connect(this, &MainWindow::aborted, res, &QNetworkReply::abort);

    //Loading.
    m_friendsView->setProperty("status", 1);
}

void MainWindow::onFriendsGetFinished()
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> res(
                static_cast<QNetworkReply*>(sender()));
    if (res->error() == QNetworkReply::NoError)
    {
        bool ok = false;
        const auto val = VKResponse::responseOrError(res.get()->readAll(), ok);
        res.reset();
        if (ok)
        {
            auto watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this,
                    &MainWindow::onFriendsFillFinished);
            connect(watcher, &QFutureWatcher<void>::finished, watcher,
                    &QFutureWatcher<void>::deleteLater);

            auto it = m_tasks.insert(watcher, 0);
            auto future = QtConcurrent::run(this, &MainWindow::fillFriends,
                                            val.toObject().value("items").toArray(), it.value());
            watcher->setFuture(future);
        }
        else
        {
            //Error.
            m_friendsView->setProperty("error", val.toObject().value("error_msg").toString());
            m_friendsView->setProperty("status", 3);
        }
    }
    else
    {
        //Handel error.
    }
}

//async.
void MainWindow::fillFriends(const QJsonArray &array, const QAtomicInteger<char> &abort)
{
    for (const auto &val : array)
    {
        //Relaxed.
        if (abort.load())
            break;
        else
        {
            BasicItem *item = MainWindow::createUserItem(val, QStringLiteral("first_name"));
            //Threadsafe if only fill model.
            m_friends->appendRow(item);
        }
    }
}

void MainWindow::onFriendsFillFinished()
{
    const auto watched = static_cast<QFutureWatcher<void>*>(sender());
    m_tasks.remove(watched);
    if (!watched->isCanceled())
    {
        QModelIndex index;
        //Threadsafe if only fill model.
        while(m_friends->canFetchMore(index))
            m_friends->fetchMore(index);

        m_friendsView->setProperty("status", 2);
    }
}

void MainWindow::onGroupsViewItemRequested()
{
    //User groupsGetById.
    //const auto row = sender()->property("currentIndex").toInt();
    //Threadsafe if only fill model.
    //int id = static_cast<VKItem*>(m_groups->itemFromRow(row))->id();
    //m_logHistory = true;
    //usersGet(id);
}

void MainWindow::onUrlToClipboardTriggered()
{
    const auto i = m_audiosView->property("currentIndex").toInt();
    const auto item = m_audios->itemFromRow<VKItem>(i);
    switch (item->sourceStatus())
    {
    case VKItem::ReadyStatus:
        QGuiApplication::clipboard()->setText(item->source().toString());
        break;
    case VKItem::NoStatus:
        requestAudioSource(i);
        break;
    }
}

void MainWindow::requestAudioSource(int i)
{
    //Threadsafe if only fill model.
    const auto section = m_audios->audioReloadSection(m_audios->index(i, 0));
    if (!section.first.empty())
    {
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("act"), QStringLiteral("reload_audio"));
        query.addQueryItem(QStringLiteral("al"), QStringLiteral("1"));
        query.addQueryItem(QStringLiteral("ids"), section.first.join(','));

        auto res = m_vk->request(QStringLiteral("al_audio.php"), query);
        connect(res, &QNetworkReply::finished,
                [this, res, section]
        {
            onReloadSectionFinished(res, section);
        });
        connect(this, &MainWindow::aborted, res, &QNetworkReply::abort);
    }
}

void MainWindow::onPlaylistCurrentChaged()
{
    const auto index = m_playlist->currentIndex();
    if (index.isValid())
    {
        const auto item = m_audios->itemFromIndex<VKItem>(index);
        switch (item->sourceStatus())
        {
        case VKItem::ReadyStatus:
            m_player->setProperty("source", item->source());
            m_playbackControl->setProperty("title", item->text());
            m_playbackControl->setProperty("duration", item->metadata());
            QMetaObject::invokeMethod(m_player, "stop");
            QMetaObject::invokeMethod(m_player, "play");
            break;
        case VKItem::NoStatus:
            requestAudioSource(index.row());
            break;
        }
    }
}

void MainWindow::onReloadSectionFinished(QNetworkReply *reply, const VKResponse::Section &section)
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> res(reply);
    if (res->error() == QNetworkReply::NoError)
    {
        auto data = res->readAll();
        const auto doc = VKResponse::parseAudioReloadSection(
                    data, res->header(QNetworkRequest::ContentTypeHeader).toString());
        res.reset();

        auto list = doc.array();
        const auto pList = new QJsonArray(list);
        auto watcher = new QFutureWatcher<QJsonArray>(this);
        connect(watcher, &QFutureWatcher<QJsonArray>::finished,
                [this, watcher, section]
        {
            m_tasks.remove(watcher);
            if (!watcher->isCanceled())
                onDecodeAudioSectionFinished(watcher->result(), section);
        });
        connect(watcher, &QFutureWatcher<QJsonArray>::finished, watcher,
                &MainWindow::deleteLater);
        connect(watcher, &QFutureWatcher<QJsonArray>::destroyed, [pList] { delete pList; });

        m_tasks.insert(watcher, 0);
        auto future = QtConcurrent::mappedReduced(*pList, MainWindow::mapAudioUrl,
                                                  MainWindow::reduceAudioUrls,
                                                  QtConcurrent::OrderedReduce);
        watcher->setFuture(future);
    }
    else
    {
        //Handle error.
    }
}

void MainWindow::onDecodeAudioSectionFinished(QJsonArray list, const VKResponse::Section &section)
{
    //Ensure size of list like in section.
    if (list.size() < section.first.size())
    {
        for (int i = 0, size = section.first.size(); i < size; ++i)
        {
            const auto id = section.first.at(i).split('_').at(1).toInt();
            if (list.at(i).toArray().at(0).toInt() != id)
                list.insert(i, QJsonValue());
        }
    }

    int j = 0;
    for (const auto &range : section.second)
    {
        for (int i = range.first; i <= range.second; ++i, ++j)
        {
            const auto array = list.at(j).toArray();
            if (array.at(2).toString().isEmpty())
            {
                //Threadsafe if only fill model.
                m_audios->itemFromRow<VKItem>(i)->setSourceStatus(VKItem::UnavailableStatus);
            }
            else
            {
                //Threadsafe if only fill model.
                auto item = m_audios->itemFromRow<VKItem>(i);
                item->setSource(array.at(2).toString(), false);
                item->setSourceStatus(VKItem::ReadyStatus, false);
                const auto index = m_audios->indexFromItem(item);
                QVector<int> roles{VKItemModel::SourceRole, VKItemModel::SourceStatusRole};
                m_audios->dataChanged(index, index, roles);
                if (item->row() == m_playlist->current())
                {
                    m_player->setProperty("source", item->source());
                    m_playbackControl->setProperty("title", item->text());
                    m_playbackControl->setProperty("duration", item->metadata());
                    QMetaObject::invokeMethod(m_player, "stop");
                    QMetaObject::invokeMethod(m_player, "play");
                }
            }
        }
    }
}

void MainWindow::groupsGet(int id, int offset)
{
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("user_id"), QString::number(id));
    query.addQueryItem(QStringLiteral("fields"), QStringLiteral("photo_100"));
    query.addQueryItem(QStringLiteral("extended"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("offset"), QString::number(offset));

    auto res = m_vk->apiRequest(QStringLiteral("groups.get"), query);
    connect(res, &QNetworkReply::finished, this, &MainWindow::onGroupsGetFinished);
    connect(this, &MainWindow::aborted, res, &QNetworkReply::abort);

    //Loading.
    m_groupsView->setProperty("status", 1);
}

void MainWindow::onGroupsGetFinished()
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> res(
                static_cast<QNetworkReply*>(sender()));
    if (res->error() == QNetworkReply::NoError)
    {
        bool ok = false;
        const auto val = VKResponse::responseOrError(res.get()->readAll(), ok);
        res.reset();
        if (ok)
        {
            auto watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this,
                    &MainWindow::onGroupsFillFinished);
            connect(watcher, &QFutureWatcher<void>::finished, watcher,
                    &QFutureWatcher<bool>::deleteLater);

            auto it = m_tasks.insert(watcher, 0);
            auto future = QtConcurrent::run(this, &MainWindow::fillGroups,
                                            val.toObject().value("items").toArray(), it.value());
            watcher->setFuture(future);
        }
        else
        {
            //Error.
            m_groupsView->setProperty("error", val.toObject().value("error_msg").toString());
            m_groupsView->setProperty("status", 3);
        }
    }
}

//async.
void MainWindow::fillGroups(const QJsonArray &array, const QAtomicInteger<char> &abort)
{
    for (const auto &val : array)
    {
        //Relaxed.
        if (abort.load())
            break;
        else
        {
            BasicItem *item = MainWindow::createUserItem(val, QStringLiteral("name"));
            //Threadsafe if only fill model.
            m_groups->appendRow(item);
        }
    }
}

void MainWindow::onGroupsFillFinished()
{
    auto watched = static_cast<QFutureWatcher<void>*>(sender());
    m_tasks.remove(watched);
    if (!watched->isCanceled())
    {
        QModelIndex index;
        //Threadsafe if only fill model.
        while(m_groups->canFetchMore(index))
            m_groups->fetchMore(index);

        m_groupsView->setProperty("status", 2);
    }
}

void MainWindow::audiosGet(int id, int offset)
{   
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("owner_id"), QString::number(id));
    query.addQueryItem(QStringLiteral("act"), QStringLiteral("load_section"));
    query.addQueryItem(QStringLiteral("al"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("playlist_id"), QStringLiteral("-1"));
    query.addQueryItem(QStringLiteral("type"), QStringLiteral("playlist"));
    query.addQueryItem(QStringLiteral("offset"), QString::number(offset));

    auto res = m_vk->request(QStringLiteral("al_audio.php"), query);
    connect(res, &QNetworkReply::finished, this, &MainWindow::onAudiosGetFinished);
    connect(this, &MainWindow::aborted, res, &QNetworkReply::abort);

    //Loading.
    m_audiosView->setProperty("status", 1);
}

void MainWindow::onAudiosGetFinished()
{
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> res(
                static_cast<QNetworkReply*>(sender()));
    if (res->error() == QNetworkReply::NoError)
    {
        //Do it witch watcher, because this json doc is very big.
        auto watcher = new QFutureWatcher<QJsonDocument>(this);
        connect(watcher, &QFutureWatcher<QJsonDocument>::finished, this,
                &MainWindow::onParseAudioSectionFinished);
        connect(watcher, &QFutureWatcher<QJsonDocument>::finished, watcher,
                &QFutureWatcher<QJsonDocument>::deleteLater);

        m_tasks.insert(watcher, 0);
        auto future = QtConcurrent::run<QJsonDocument>(
                    VKResponse::parseAudioSection, res->readAll(),
                    res->header(QNetworkRequest::ContentTypeHeader).toString(), nullptr);
        res.reset();
        watcher->setFuture(future);
    }
    else
    {
        //Handle error.
        //Check m_firstAudios and if it true show error, if it false show error in view's footer.
    }
}

void MainWindow::onParseAudioSectionFinished()
{
    auto watched = static_cast<QFutureWatcher<QJsonDocument>*>(sender());
    m_tasks.remove(watched);
    if (watched->isCanceled())
        return;

    const auto doc = watched->result();
    if (!doc.isNull())
    {
        const auto obj = doc.object();
        const auto list = obj.value("list").toArray();

        //First 100 audios always has encoded links.
        if (m_firstAudios)
        {
            const auto pList = new QJsonArray(list);
            auto watcher = new QFutureWatcher<QJsonArray>(this);
            connect(watcher, &QFutureWatcher<QJsonArray>::finished, this,
                    &MainWindow::onDecodeAudioUrlsFinished);
            connect(watcher, &QFutureWatcher<QJsonArray>::finished, watcher,
                    &MainWindow::deleteLater);
            connect(watcher, &QFutureWatcher<QJsonArray>::destroyed, [pList] { delete pList; });

            m_tasks.insert(watcher, 0);
            auto future = QtConcurrent::mappedReduced(*pList, MainWindow::mapAudioUrl,
                                                      MainWindow::reduceAudioUrls,
                                                      QtConcurrent::OrderedReduce);
            watcher->setFuture(future);
        }
        else
        {
            auto watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this,
                    &MainWindow::onAudiosFillFinished);
            connect(watcher, &QFutureWatcher<void>::finished, watcher,
                    &QFutureWatcher<void>::deleteLater);

            auto it = m_tasks.insert(watcher, 0);
            auto future = QtConcurrent::run(this, &MainWindow::fillAudios, list, it.value(),
                                            false);
            watcher->setFuture(future);
        }
        m_firstAudios = false;

        //Can be unordered result.
        auto offset = obj.value("nextOffset").toInt();
        auto total = obj.value("totalCount").toInt();
        if (offset < total)
            audiosGet(m_id, offset);
    }
    else
    {
        //Error.
        m_audiosView->setProperty("error", "Can't load audio section. Try to relogin.");
        m_audiosView->setProperty("status", 3);
    }
}

//static. async. QtConcurrent::mapped.
QJsonValue MainWindow::mapAudioUrl(const QJsonValue &val)
{
    auto audio = val.toArray();
    audio[2] = VKResponse::decodeAudioUrl(88986886, audio.at(2).toString());
    return audio;
}

//static. async. QtConcurrent::mappedReduced.
void MainWindow::reduceAudioUrls(QJsonArray &list, const QJsonValue &val)
{
    list.append(val);
}

void MainWindow::onDecodeAudioUrlsFinished()
{
    auto watched = static_cast<QFutureWatcher<QJsonArray>*>(sender());
    m_tasks.remove(watched);
    if (watched->isCanceled())
        return;

    const auto list = watched->result();
    auto watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, &MainWindow::onAudiosFillFinished);
    connect(watcher, &QFutureWatcher<void>::finished, watcher,
            &QFutureWatcher<void>::deleteLater);

    auto it = m_tasks.insert(watcher, 0);
    auto future = QtConcurrent::run(this, &MainWindow::fillAudios, list, it.value(), true);
    watcher->setFuture(future);
}

//async.
void MainWindow::fillAudios(const QJsonArray &list, const QAtomicInteger<char> &abort,
                            bool considerSource)
{
    for (const auto &audio : list)
    {
        //Relaxed.
        if (abort.load())
            break;

        const auto array = audio.toArray();

        auto item = new VKItem(array.at(1).toInt(),
                               Utilities::normalizeHtml(array.at(4).toString())
                               % " - "
                               % Utilities::normalizeHtml(array.at(3).toString()));

        item->setContentId(array.at(0).toInt());
        item->setMetadata(Utilities::secondsToString(array.at(5).toInt()));
        item->setAudioHash(array.at(13).toString().split('/'));
        const auto covers = array.at(14).toString().split(',', QString::SkipEmptyParts);
        if (covers.size() > 1)
            item->setCover(QUrl(covers.at(1)));
        else
            item->setCover(QUrl("qrc:/icons/breeze/mimetypes/22/audio-x-generic"));

        const auto src = array.at(2).toString();
        item->setSource(QUrl(src));
        if (src.isEmpty())
        {
            if (considerSource)
                item->setSourceStatus(VKItem::UnavailableStatus);
            else
                item->setSourceStatus(VKItem::NoStatus);
        }
        else
            item->setSourceStatus(VKItem::ReadyStatus);

        //Threadsafe if only fill model.
        m_audios->appendRow(item);
    }
}

void MainWindow::onAudiosFillFinished()
{
    auto watched = static_cast<QFutureWatcher<void>*>(sender());
    m_tasks.remove(watched);
    if (!watched->isCanceled())
    {
        QModelIndex index;
        //Threadsafe if only fill model.
        while(m_audios->canFetchMore(index))
            m_audios->fetchMore(index);

        m_audiosView->setProperty("status", 2);
    }
}

void MainWindow::onLoginTriggered()
{
    //No need anymore.
    //If an app links statically with lib containing resource.
    //Download that resource (login.qrc).
    //Q_INIT_RESOURCE(login);

    auto dialog = std::make_unique<VKLoginDialogQml>(
                m_engine, QLatin1Literal("5571207"), QLatin1Literal("5.100"),
                QStringList{QStringLiteral("audio"),
                            QStringLiteral("video"),
                            QStringLiteral("groups"),
                            QStringLiteral("offline")});
    if (dialog->exec())
    {
        m_vk->setApiVersion(dialog->version());
        m_vk->setAccessToken(dialog->accessToken());
        auto cookieJar = m_vk->networkAccessManager()->cookieJar();
        for (const auto &cookie : dialog->cookies())
            cookieJar->insertCookie(cookie);

        m_logHistory = true;
        usersGet();
    }
    else
    {
        //Handle failed login.
    }

    //No need anymore.
    //If an app links statically with lib containing resource.
    //Clear that resource (login.qrc).
    //Q_CLEANUP_RESOURCE(login);
}
