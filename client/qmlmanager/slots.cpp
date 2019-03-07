#include "slots.hpp"
#include "localstorage.hpp"
#include "pppoe.hpp"
#include "qmlwindowsmanager.hpp"
#include "utils.hpp"

void Slots::start(QString username, QString passwd, QString pack_info,
                  QString NIC_info, QString remPass, QString autoLogin) {
    qDebug() << "start pppoe";
    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        auto password = remPass == "t" ? passwd : QString{""};
        profile->setLoginInfo(username, password, pack_info);
        if (autoLogin == "t") {
            profile->setAutoLoginUser(username);
        }
        profile->close();
    }
    QString model = utils::getDrModel(pack_info);
    auto realUsername = "\r\n" + username + model;
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->dialRAS(NDR_PHONEBOOK_NAME, realUsername, passwd, NIC_info);
    pppoe->start();
}

void Slots::dailFnish(const bool &finish) {
    if (finish == true) {
        auto pppoe = utils::resourceManager.getPPPoE();
        QMetaObject::invokeMethod(
            const_cast<QWindow *>(utils::resourceManager.getWindow()),
            "dailSeccess", Qt::DirectConnection,
            Q_ARG(QVariant, QVariant{pppoe->getIpAddress()}));
    } else {
        auto pppoe = utils::resourceManager.getPPPoE();
        auto errorInfo = pppoe->lastError();
        if (errorInfo == "") {
            errorInfo = "未知错误";
        }
        QMetaObject::invokeMethod(
            const_cast<QWindow *>(utils::resourceManager.getWindow()),
            "dailField", Qt::DirectConnection,
            Q_ARG(QVariant, QVariant{errorInfo}));
    }
}

void Slots::stopConnect() {
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->hangUp();
}
