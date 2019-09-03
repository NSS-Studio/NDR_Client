#include "setconfig.hpp"
#include "localstorage.hpp"
#include "pppoe.hpp"
#include "utils.hpp"
#include <QMessageBox>
#include <QMetaObject>

#include <iostream>

bool setConfig::setUsername(QWindow *window) {
    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        auto vec = profile->getUserNameList();
        for (const auto &tmp : vec) {
            QMetaObject::invokeMethod(window, "addUsernameInfo",
                                      Qt::DirectConnection,
                                      Q_ARG(QVariant, QVariant{tmp}));
        }
    } else {
        return false;
    }
    profile->close();
    return true;
}

bool setConfig::setPackInfo(QWindow *window) {
    QStringList postfitList = utils::getDrModelPostfixTable();

    for (auto const &postfit : postfitList) {
        qDebug() << postfit;
        QMetaObject::invokeMethod(window, "addPackInfo", Qt::DirectConnection,
                                  Q_ARG(QVariant, QVariant{postfit}));
    }
    return true;
}

bool setConfig::setDevice(QWindow *window) {
    auto pppoe = utils::resourceManager.getPPPoE();
    QStringList interfaces = pppoe->getAvailableInterfaces();
    for (const auto &interface : interfaces) {
        QMetaObject::invokeMethod(window, "addDevice", Qt::DirectConnection,
                                  Q_ARG(QVariant, QVariant{interface}));
    }
    return true;
}

bool setConfig::setTittle(QWindow *window) {
//    QMetaObject::invokeMethod(
//        window, "setVersion", Qt::DirectConnection,
//        Q_ARG(QVariant, QVariant{QString{"NDR东软网络客户端"} +
//                                 utils::getVersionString()}));
        QMetaObject::invokeMethod(
            window, "setVersion", Qt::DirectConnection,
            Q_ARG(QVariant, QVariant{QString{"N-Client Alpha 0.1"}}));

    return true;
}

bool setConfig::setLastLogin(QWindow *window) {
    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        QString user = {};
        profile->getLastLoginUser(user);
        QMetaObject::invokeMethod(window, "setLastLoginUser",
                                  Qt::DirectConnection,
                                  Q_ARG(QVariant, QVariant{user}));
    } else {
        return false;
    }
    profile->close();
    return true;
}

bool setConfig::setLoginInfo(QWindow *window, QString username, bool status) {
    if (status) {
        QMetaObject::invokeMethod(window, "addLoginInfo", Qt::DirectConnection,
                                  Q_ARG(QVariant, QVariant{QString{""}}),
                                  Q_ARG(QVariant, QVariant{QString{""}}));
    } else {
        auto profile = utils::resourceManager.getProfile();
        if (profile->open()) {
            QString user, password, manner;
            if (username == "") {
                profile->getLastLoginUser(user);
            } else {
                user = username;
            }
            profile->getLoginInfo(user, password, manner);

            std::cout << user.toStdString() + password.toStdString() + manner.toStdString()<<std::endl;
            QMetaObject::invokeMethod(window, "addLoginInfo",
                                      Qt::DirectConnection,
                                      Q_ARG(QVariant, QVariant{password}),
                                      Q_ARG(QVariant, QVariant{manner}));

        } else {
            return false;
        }
        profile->close();
    }
    return true;
}
