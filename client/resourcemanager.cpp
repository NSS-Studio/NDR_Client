#include "resourcemanager.hpp"
#include <utils.hpp>
#include <pppoe.hpp>
#include <mainwindow.h>
#include <logindialog.h>
#include <localstorage.hpp>
#include <popupinfomationdialog.h>

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager() {

}

void ResourceManager::InitResourceManager() {
    pppoe.reset(new PPPoE{});
    profile.reset(new LocalStorage{utils::appHome + "/config.db"});
    loginDialog.reset(new LoginDialog{profile});
    mainWindow.reset(new MainWindow{profile});
    popUpInfomationDialog.reset(new PopUpInfomationDialog{});

#ifndef QT_DEBUG
    mainWindow->show();
#endif
    loginDialog->show();
}

QSharedPointer<PPPoE> ResourceManager::getPPPoE() {
    return pppoe;
}

QSharedPointer<LoginDialog> ResourceManager::getLoginDialog() {
    return loginDialog;
}

QSharedPointer<PopUpInfomationDialog> ResourceManager::getPopUpInfomationDialog() {
    return popUpInfomationDialog;
}
