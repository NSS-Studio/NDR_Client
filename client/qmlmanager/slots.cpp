#include <QMessageBox>

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
            this->autoLogin = true;
            rember = true;
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
            profile->setLoginInfo(username, password, manner);
        } else {
            profile->setLoginInfo(username, QString{""}, manner);
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

void Slots::stopConnect(int flag) {
    qDebug() << "stop";
    if (flag == 1){
        auto pppoe = utils::resourceManager.getPPPoE();
        pppoe->hangUp();
        Sleep(800);
        QApplication::exit();
    }
    else {
        auto pppoe = utils::resourceManager.getPPPoE();
        pppoe->hangUp();
    }

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


#ifdef Q_OS_WIN
void Slots::resetWinsock() {
  HINSTANCE re;
  intptr_t reValue;
  re = ShellExecute(NULL, TEXT("runas"), TEXT("cmd"), TEXT("/C netsh winsock reset"), TEXT(""), SW_HIDE);
  // can not use static_cast
  // Because int is 4-byte HINSTANCE, is known as PVOID, is 8-byte
  // So We should use intptr_t
  reValue = reinterpret_cast<intptr_t>(re);
  if (reValue <= 32) {
    QMessageBox::information(NULL, tr("失败"), tr("Winsock重置失败，错误代码 %0").arg(reValue));
    return;
  }

  re = ShellExecute(NULL, TEXT("runas"), TEXT("cmd"), TEXT("/C netsh winhttp reset proxy"),TEXT(""), SW_HIDE);
  reValue = reinterpret_cast<intptr_t>(re);
  if (reValue <= 32) {
    QMessageBox::information(NULL, tr("失败"), tr("网络代理重置失败，错误代码 %0").arg(reValue));
    return;
  }

  re = ShellExecute(NULL, TEXT("runas"), TEXT("cmd"), TEXT("/C ipconfig /flushdns"), TEXT(""), SW_HIDE);
  if (reValue <= 32) {
    QMessageBox::information(NULL, tr("失败"), tr("DNS重置失败，错误代码 %0").arg(reValue));
    return;
  }

  QMessageBox::information(NULL, tr("成功"), tr("网络重置成功"));
}

#endif
