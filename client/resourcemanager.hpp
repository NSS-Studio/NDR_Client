#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <QSharedPointer>
class PPPoE;
class MainWindow;
class LoginDialog;
class LocalStorage;
class PopUpInfomationDialog;
class AboutDialog;
class ResourceManager final
{
public:
    ResourceManager();
    ~ResourceManager();
    void InitResourceManager();
    QSharedPointer<PPPoE> getPPPoE();
    QSharedPointer<LoginDialog> getLoginDialog();
//    QSharedPointer<PopUpInfomationDialog> getPopUpInfomationDialog();
    QSharedPointer<AboutDialog> getAboutDialog();
    QSharedPointer<LocalStorage> getProfile();
private:
    QSharedPointer<PPPoE> pppoe;
    QSharedPointer<MainWindow> mainWindow;
    QSharedPointer<LocalStorage> profile;
    QSharedPointer<LoginDialog> loginDialog;
    QSharedPointer<AboutDialog> aboutDialog;
//    QSharedPointer<PopUpInfomationDialog> popUpInfomationDialog;
};

#endif // RESOURCEMANAGER_HPP
