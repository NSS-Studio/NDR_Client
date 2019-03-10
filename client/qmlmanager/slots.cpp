#include "slots.hpp"
#include "localstorage.hpp"
#include "pppoe.hpp"
#include "qmlwindowsmanager.hpp"
#include "setconfig.hpp"
#include "utils.hpp"

void Slots::start(QString username, QString passwd, QString pack_info,
                  QString NIC_info, QString remPass, QString autoLogin) {
    this->username = username;
    this->password = passwd;
    this->manner = pack_info;
    this->devicename = NIC_info;
    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        if (remPass == "t") {
            rember = true;
        }

        if (autoLogin == "t") {
            autoLogin = true;
        }
        profile->close();
    }
    QString model = utils::getDrModel(pack_info);
    auto realUsername = "\r\n" + username + model;
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->dialRAS(NDR_PHONEBOOK_NAME, realUsername, passwd, NIC_info);
    pppoe->start();
}

void Slots::dailFinish(const bool &finish) {
    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        if (rember) {
            profile->setLoginInfo(username, password, devicename);
        } else {
            profile->setLoginInfo(username, QString{""}, devicename);
        }
        if (autoLogin) {
            profile->setAutoLoginUser(username);
        }
        profile->close();
    }
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
    qDebug() << "stop";
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->hangUp();
}

void Slots::changtAccount(QString account) {
    auto window = utils::resourceManager.getWindow();
    setConfig::setLoginInfo(window, account);
}

void Slots::clearConfig() {
    QDir home = utils::appHome;
    DeleteDirectory(QDir::homePath() + "/.nssdr");
    setConfig::setLoginInfo(utils::resourceManager.getWindow(), QString{""},
                            true);
}

bool Slots::DeleteDirectory(const QString &path) {
    if (path.isEmpty()) {
        return false;
    }

    QDir dir(path);
    if (!dir.exists()) {
        return true;
    }

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList) {
        if (fi.isFile()) {
            fi.dir().remove(fi.fileName());
        } else {
            DeleteDirectory(fi.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}
