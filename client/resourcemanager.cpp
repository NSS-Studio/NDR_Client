#include "resourcemanager.hpp"
#include <utils.hpp>
#include <pppoe.hpp>
#include <mainwindow.h>
#include <logindialog.h>
#include <localstorage.hpp>

ResourceManager::ResourceManager()
{

}

void ResourceManager::InitResourceManager() {
    pppoe.reset(new PPPoE{});
    mainWindow.reset(new MainWindow{sharedFromThis()});
    profile.reset(new LocalStorage{utils::appHome + "/config.db"});
    loginDialog.reset(new LoginDialog{profile,sharedFromThis()});

    mainWindow->show();
    loginDialog->show();
}

QSharedPointer<PPPoE> ResourceManager::getPPPoE() {
    return pppoe;
}

QSharedPointer<LoginDialog> ResourceManager::getLoginDialog() {
    return loginDialog;
}
