#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <QSharedPointer>
class PPPoE;
class MainWindow;
class LoginDialog;
class LocalStorage;
class ResourceManager final
{
public:
    ResourceManager();
    ~ResourceManager();
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
