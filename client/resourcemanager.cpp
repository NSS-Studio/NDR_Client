#include "resourcemanager.hpp"
#include "aboutdialog.hpp"
#include "popupinfomationdialog.hpp"
#include <QMutexLocker>
#include <QWindow>
#include <localstorage.hpp>
#include <logindialog.hpp>
#include <mainwindow.hpp>
#include <pppoe.hpp>
#include <utils.hpp>

ResourceManager::~ResourceManager() {
    if (initState == InitState::DONE) {
        pppoe->deleteLater();
        mainWindow->deleteLater();
        profile->deleteLater();
        loginDialog->deleteLater();
        aboutDialog->deleteLater();
        popUpInfomationDialog->deleteLater();
        initState = InitState::UNDONE;
    }
}

void ResourceManager::InitResourceManager() noexcept {
    QMutexLocker locker{&mutex};
//    aboutDialog = new AboutDialog{};
    pppoe = new PPPoE{};
    profile = new LocalStorage{utils::appHome + "/config.db"};
//    loginDialog = new LoginDialog{}; // close old ui
//    mainWindow = new MainWindow{};
//    popUpInfomationDialog = new PopUpInfomationDialog{};
    initState = InitState::DONE;
#ifdef QT_DEBUG
//  mainWindow->show();
#endif
    //  loginDialog->show();
}

PPPoE *ResourceManager::getPPPoE() const noexcept { return pppoe; }

LoginDialog *ResourceManager::getLoginDialog() const noexcept {
    return loginDialog;
}

PopUpInfomationDialog *ResourceManager::getPopUpInfomationDialog() const
    noexcept {
    return popUpInfomationDialog;
}

AboutDialog *ResourceManager::getAboutDialog() const noexcept {
    return aboutDialog;
}

LocalStorage *ResourceManager::getProfile() const noexcept { return profile; }

void ResourceManager::setWindow(const QWindow *win) noexcept {
    this->window = const_cast<QWindow *>(win);
}
QWindow *ResourceManager::getWindow() const noexcept { return this->window; }
