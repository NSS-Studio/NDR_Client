#include "setconfig.hpp"
#include "localstorage.hpp"
#include "pppoe.hpp"
#include "utils.hpp"
#include <QMetaObject>

bool setConfig::setUsername(QWindow *window) {
    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        auto vec = profile->getUserNameList();
        for (const auto &tmp : vec) {
            QMetaObject::invokeMethod(window, "addUsernameInfo",
                                      Qt::DirectConnection,
                                      Q_ARG(QVariant, QVariant{tmp}));
        }
        return true;
    } else {
        return false;
    }
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
}

bool setConfig::setTittle(QWindow *window) {
    QMetaObject::invokeMethod(
        window, "setVersion", Qt::DirectConnection,
        Q_ARG(QVariant, QVariant{QString{"NDR东软网络客户端"} +
                                 utils::getVersionString()}));
}
