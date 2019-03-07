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
    setConfig::setUsername(const_cast<QWindow *>(this->managerWindows.get()));
    setConfig::setPackInfo(const_cast<QWindow *>(this->managerWindows.get()));
    setConfig::setDevice(const_cast<QWindow *>(this->managerWindows.get()));
    setConfig::setTittle(const_cast<QWindow *>(this->managerWindows.get()));
    setConfig::setLastLogin(const_cast<QWindow *>(this->managerWindows.get()));
    setConfig::setLoginInfo(const_cast<QWindow *>(this->managerWindows.get()));

    this->bind_slot();
}

void QMLWindowsManager::bind_slot() {
    QObject::connect(
        this->managerWindows.get(),
        SIGNAL(login(QString, QString, QString, QString, QString, QString)),
        this->slot.get(),
        SLOT(start(QString, QString, QString, QString, QString, QString)));
    auto pppoe = utils::resourceManager.getPPPoE();
    connect(pppoe, &PPPoE::dialFinished, this->slot.get(), &Slots::dailFnish,
            Qt::QueuedConnection);
}
