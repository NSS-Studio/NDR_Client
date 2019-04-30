#include "resourcemanager.hpp"
#include <QMutexLocker>
#include <QWindow>
#include <localstorage.hpp>
#include <pppoe.hpp>
#include <utils.hpp>

ResourceManager::~ResourceManager() {
    if (initState == InitState::DONE) {
        pppoe->deleteLater();
        profile->deleteLater();
        initState = InitState::UNDONE;
    }
}

void ResourceManager::InitResourceManager() noexcept {
    QMutexLocker locker{&mutex};
    pppoe = new PPPoE{};
    profile = new LocalStorage{utils::appHome + "/config.db"};
    initState = InitState::DONE;
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
