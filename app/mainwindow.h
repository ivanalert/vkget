#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vkresponse.h"
#include "navigationlog.h"
#include "basicitemmodel.h"

#include <limits>
#include <QObject>
#include <QMap>
#include <QSet>
#include <QJsonArray>
#include <QJsonValue>
#include <QVector>
#include <QSortFilterProxyModel>

class VKontakte;
class BasicItem;
class Playlist;
class DownloadManager;
class QQmlEngine;
class QQuickItem;
class QQuickWindow;
class QNetworkReply;

class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow() override;

    QQmlEngine* engine() const noexcept
    {
        return m_engine;
    }

signals:
    void aborted();

private slots:
    void onLoginTriggered();

    void onPreviousTriggered()
    {
        m_logHistory = false;
        usersGet(m_navLog->previous());
    }

    void onNextTriggered()
    {
        m_logHistory = false;
        usersGet(m_navLog->next());
    }

    void onHomeTriggered()
    {
        if (m_navLog->hasPrevious())
        {
            m_logHistory = false;
            usersGet(m_navLog->home());
        }
    }

    void onFilterEdited();

    void onDownloadAudioTriggered();
    void onUrlToClipboardTriggered();

    void logNavigation()
    {
        m_navLog->record(m_id);
    }

    void usersGet()
    {
        usersGet(m_id);
    }

    void onUsersGetFinished();
    void onFriendsGetFinished();
    void onGroupsGetFinished();
    void onAudiosGetFinished();

    void onFriendsFillFinished();
    void onGroupsFillFinished();
    void onAudiosFillFinished();

    void onFriendsViewItemRequested();
    void onGroupsViewItemRequested();
    void onPlaylistCurrentChaged();

    void onParseAudioSectionFinished();
    void onReloadSectionFinished(QNetworkReply *reply, const VKResponse::Section &section);
    void onDecodeAudioUrlsFinished();
    void onDecodeAudioSectionFinished(QJsonArray list, const VKResponse::Section &section);

private:
    template<typename T>
    T* sourceModel(QSortFilterProxyModel *model) const
    {
        return static_cast<T*>(model->sourceModel());
    }

    template<typename T>
    T* sourceItem(QSortFilterProxyModel *model, int row) const
    {
        return sourceItem<T>(model, model->index(row, 0));
    }

    template<typename T>
    T* sourceItem(QSortFilterProxyModel *model, const QModelIndex &proxy) const
    {
        const auto index = model->mapToSource(proxy);
        return sourceModel<BasicItemModel>(model)->itemFromIndex<T>(index);
    }

    static BasicItem* createUserItem(const QJsonValue &value, const QString &name);

    void fillFriends(const QJsonArray &array, const QAtomicInteger<char> &abort);
    void fillGroups(const QJsonArray &array, const QAtomicInteger<char> &abort);
    void fillAudios(const QJsonArray &list, const QAtomicInteger<char> &abort,
                    bool considerSource);

    //QtConcurrent::mapped functions.
    static QJsonValue mapAudioUrl(const QJsonValue &val);
    static void reduceAudioUrls(QJsonArray &list, const QJsonValue &val);

    void abortTasks();

    void usersGet(int id);
    void friendsGet(int id, int offset = 0);
    void groupsGet(int id, int offset = 0);
    void audiosGet(int id, int offset = 0);
    void realoadSection(int index, int secIndex);
    void requestAudioSource(int i);

    QQmlEngine *m_engine;
    bool loggedin = false;
    VKontakte *m_vk = nullptr;

    QQuickWindow *m_root = nullptr;
    QObject *m_navigation = nullptr;
    QQuickItem *m_friendsView = nullptr;
    QQuickItem *m_groupsView = nullptr;
    QQuickItem *m_audiosView = nullptr;
    QQuickItem *m_playbackControl = nullptr;
    QObject *m_player = nullptr;

    //Models guarded by ThreadsafeNode.
    QSortFilterProxyModel *m_friends;
    QSortFilterProxyModel *m_groups;
    QSortFilterProxyModel *m_audios;
    Playlist *m_playlist;
    QSortFilterProxyModel *m_downloads;
    DownloadManager *m_downloadManager;

    int m_id = std::numeric_limits<int>::max();

    QMap<QObject*, QAtomicInteger<char>> m_tasks;
    NavigationLog *m_navLog;

    bool m_firstAudios = true;
    bool m_logHistory = true;
};

#endif // MAINWINDOW_H
