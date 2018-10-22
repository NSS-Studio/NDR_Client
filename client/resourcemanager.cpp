#include "resourcemanager.hpp"
#include <utils.hpp>
#include <pppoe.hpp>
#include <mainwindow.h>
#include <logindialog.h>
#include <localstorage.hpp>
#include "popupinfomationdialog.h"
#include "aboutdialog.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager() {
}

void ResourceManager::InitResourceManager() {
    aboutDialog.reset(new AboutDialog{});
    pppoe.reset(new PPPoE{});
    profile.reset(new LocalStorage{utils::appHome + "/config.db"});
    loginDialog.reset(new LoginDialog{});
    mainWindow.reset(new MainWindow{});
//    popUpInfomationDialog.reset(new PopUpInfomationDialog{});
#ifdef QT_DEBUG
    mainWindow->show();
#endif
    loginDialog->show();
}

QSharedPointer<PPPoE> ResourceManager::getPPPoE() {
    return pppoe->sharedFromThis();
}

QSharedPointer<LoginDialog> ResourceManager::getLoginDialog() {
    return loginDialog->sharedFromThis();
}

//QSharedPointer<PopUpInfomationDialog> ResourceManager::getPopUpInfomationDialog() {
//    return popUpInfomationDialog->sharedFromThis();
//}

QSharedPointer<AboutDialog> ResourceManager::getAboutDialog() {
    return aboutDialog->sharedFromThis();
}

QSharedPointer<LocalStorage> ResourceManager::getProfile() {
    return profile->sharedFromThis();
}
