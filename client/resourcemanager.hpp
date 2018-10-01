#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

//#include <mainwindow.h>
//#include <logindialog.h>
//#include <pppoe.hpp>
#include <QSharedPointer>
#include <QEnableSharedFromThis>
class PPPoE;
class MainWindow;
class LoginDialog;
class LocalStorage;
class ResourceManager final:
        public QEnableSharedFromThis<ResourceManager>
{
public:
    ResourceManager();

    void InitResourceManager();

    QSharedPointer<PPPoE> getPPPoE();
    QSharedPointer<LoginDialog> getLoginDialog();

private:
    QSharedPointer<PPPoE> pppoe;
    QSharedPointer<MainWindow> mainWindow;
    QSharedPointer<LocalStorage> profile;
    QSharedPointer<LoginDialog> loginDialog;

};

#endif // RESOURCEMANAGER_HPP
