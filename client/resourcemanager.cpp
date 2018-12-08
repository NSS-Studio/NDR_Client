#include "resourcemanager.hpp"
#include <utils.hpp>
#include <pppoe.hpp>
#include <mainwindow.h>
#include <logindialog.h>
#include <localstorage.hpp>
#include <QMutexLocker>
#include "popupinfomationdialog.h"
#include "aboutdialog.h"

ResourceManager::~ResourceManager() {
    if (initState == InitState::TRUE) {
        pppoe->deleteLater();
        mainWindow->deleteLater();
        profile->deleteLater();
        loginDialog->deleteLater();
        aboutDialog->deleteLater();
        popUpInfomationDialog->deleteLater();
        initState = InitState::FALSE;
    }
}

void ResourceManager::InitResourceManager() noexcept{
    QMutexLocker locker{&mutex};
    aboutDialog = new AboutDialog{};
    pppoe = new PPPoE{};
    profile = new LocalStorage{utils::appHome + "/config.db"};
    loginDialog = new LoginDialog{};
    mainWindow = new MainWindow{};
    popUpInfomationDialog = new PopUpInfomationDialog{};
    initState = InitState::TRUE;
#ifdef QT_DEBUG
    mainWindow->show();
#endif
    loginDialog->show();
}

PPPoE* ResourceManager::getPPPoE() const noexcept{
    return pppoe;
}

LoginDialog* ResourceManager::getLoginDialog() const noexcept{
    return loginDialog;
}

PopUpInfomationDialog* ResourceManager::getPopUpInfomationDialog() const noexcept{
    return popUpInfomationDialog;
}

AboutDialog* ResourceManager::getAboutDialog() const noexcept{
    return aboutDialog;
}

LocalStorage* ResourceManager::getProfile() const noexcept{
    return profile;
}
