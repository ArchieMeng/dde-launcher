#ifndef APPSMANAGER_H
#define APPSMANAGER_H

#include "appslistmodel.h"
#include "dbuslauncher.h"
#include "dbusfileinfo.h"
#include "dbustartmanager.h"
#include "dbusdockedappmanager.h"
#include "dbusdisplay.h"
#include "global_util/calculate_util.h"
#include "global_util/themeappicon.h"

#include <QMap>
#include <QSettings>
#include <QPixmap>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QDBusArgument>
#include <QList>

class CalculateUtil;
class AppsManager : public QObject
{
    Q_OBJECT

public:
    static AppsManager *instance(QObject *parent = nullptr);

    void stashItem(const QModelIndex &index);
    void restoreItem(const QString &appKey, const int pos);

signals:
    void dataChanged(const AppsListModel::AppCategory category) const;
    void layoutChanged(const AppsListModel::AppCategory category) const;
    void handleUninstallApp(const QModelIndex &index, int result);
    void updateCategoryView(const AppsListModel::AppCategory categoryInfo) const;
    void primaryChanged() QT_DEPRECATED;

public slots:
    void searchApp(const QString &keywords);
    void launchApp(const QModelIndex &index);
    const ItemInfoList appsInfoList(const AppsListModel::AppCategory &category) const;

    bool appIsNewInstall(const QString &key);
    bool appIsAutoStart(const QString &desktop);
    bool appIsOnDock(const QString &appName);
    bool appIsOnDesktop(const QString &desktop);
    const QPixmap appIcon(const QString &iconKey, const int size);
    int appNums(const AppsListModel::AppCategory &category) const;

    //remove the item icon firstly, when unInstalling apps
    void unInstallApp(const QModelIndex &index, int value);
    //restore the itemInfo, if unInstall failed!
    void reStoreItem();

    QRect getPrimayRect() QT_DEPRECATED;
    void handleDragedApp(const QModelIndex &index);
    void handleDropedApp(const QModelIndex &index);

private:
    explicit AppsManager(QObject *parent = 0);

    void appendSearchResult(const QString &appKey);
    void sortCategory(const AppsListModel::AppCategory category);
    void sortByName(ItemInfoList &processList);
    void refreshCategoryInfoList();
    void refreshAppIconCache();
    void refreshAppAutoStartCache();

private slots:
    void searchDone(const QStringList &resultList);

private:
    CalculateUtil *m_calcUtil;

    DBusLauncher *m_launcherInter;
    DBusStartManager *m_startManagerInter;
    DBusDockedAppManager *m_dockedAppInter;
    DBusDisplay* m_displayInterface;

    QString m_searchText;
    QStringList m_newInstalledAppsList;
    ItemInfoList m_appInfoList;
    ItemInfoList m_appSearchResultList;
    ItemInfoList m_stashList;
    QMap<AppsListModel::AppCategory, ItemInfoList> m_appInfos;

    static AppsManager *INSTANCE;
    static QSettings APP_ICON_CACHE;
    static QSettings APP_AUTOSTART_CACHE;
    ItemInfo m_unInstallItem = ItemInfo();
    ItemInfo m_beDragedItem = ItemInfo();

    ThemeAppIcon* m_themeAppIcon;
    CalculateUtil *m_calUtil;
    QTimer *m_searchTimer;
};

#endif // APPSMANAGER_H
