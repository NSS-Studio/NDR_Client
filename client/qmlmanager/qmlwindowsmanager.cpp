#include "qmlwindowsmanager.hpp"
#include "localstorage.hpp"
#include "pppoe.hpp"
#include "setconfig.hpp"
#include "slots.hpp"

#include "utils.hpp"
#include <QMessageBox>
#include <QQmlComponent>
#include <QQuickStyle>
#include <QQuickView>
#include <QUrl>
#include <pppoe.hpp>
QMLWindowsManager::QMLWindowsManager(QObject *parent) : QObject(parent) {
    qDebug() << "qml windows manage init";
    QQuickStyle::setStyle("Universal");
    this->engineManagerDialog = QSharedPointer<QQmlApplicationEngine>::create();
    this->compManagerDialog = QSharedPointer<QQmlComponent>::create(
        this->engineManagerDialog.get(),
        QUrl{QString{"qrc:/dialog//"} + QML_PLATFROM_GET(ManagerDialog.qml)});
    this->managerWindows.reset(
        static_cast<QWindow *>(this->compManagerDialog->create()));
    this->managerWindows->show();
    this->slot = QSharedPointer<Slots>::create();
    utils::resourceManager.setWindow(managerWindows.get());
    setConfig::setUsername(this->managerWindows.get());
    setConfig::setPackInfo(this->managerWindows.get());
    setConfig::setDevice(this->managerWindows.get());
    setConfig::setTittle(this->managerWindows.get());
    setConfig::setLastLogin(this->managerWindows.get());
    setConfig::setLoginInfo(this->managerWindows.get());
    this->bind_slot();
    auto profile = utils::resourceManager.getProfile();
    QString autoLogin = QString{""};
    if (profile->open()) {
        if (profile->getAutoLoginUser(autoLogin)) {
            if (autoLogin != "") {
                QString password, device, manner;
                profile->getDeviceName(device);
                profile->getLoginInfo(autoLogin, password, manner);
                this->slot.get()->start(autoLogin, password, manner, device,
                                        QString{"f"}, QString{"f"});
                qDebug() << "autologin" + autoLogin;
            }
            qDebug() << "login: empty";
        }
        profile->close();
    }
}

void QMLWindowsManager::bind_slot() {
    QObject::connect(
        this->managerWindows.get(),
        SIGNAL(login(QString, QString, QString, QString, QString, QString)),
        this->slot.get(),
        SLOT(start(QString, QString, QString, QString, QString, QString)));
    auto pppoe = utils::resourceManager.getPPPoE();
    QObject::connect(pppoe, &PPPoE::dialFinished, this->slot.get(),
                     &Slots::dailFinish, Qt::QueuedConnection);
    QObject::connect(this->managerWindows.get(), SIGNAL(stopConnection()),
                     this->slot.get(), SLOT(stopConnect()));
    QObject::connect(this->managerWindows.get(),
                     SIGNAL(change_account_select(QString)), this->slot.get(),
                     SLOT(changtAccount(QString)));
    QObject::connect(this->managerWindows.get(), SIGNAL(clear()),
                     this->slot.get(), SLOT(clearConfig()));
}
