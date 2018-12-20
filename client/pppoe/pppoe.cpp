#include "pppoe.hpp"
#include "basedslfactory.hpp"
#include <QNetworkInterface>
#include <utils.hpp>
PPPoE::PPPoE(QObject *parent) : QThread{parent} {
  basedsl = BaseDslFactory::getCurrentPlatformBaseDsl();
  connect(this, &PPPoE::finished, this, &PPPoE::threadFinished,
          Qt::QueuedConnection);
}

QString PPPoE::lastError() { return this->errorMessage; }

void PPPoE::run() {
  // qDebug() << "PPPoE::run: device_name:" << device_name;
  // reconnect failed at here; windows bug?? when reconnect three times it will
  // success bool ret; if (isRedial)
  //    for (int i = 0; i < 3 && !ret; i++)
  //        ret = basedsl -> dial(username, password, device_name,
  //        errorMessage);
  // else
  //    ret = basedsl -> dial(username, password, device_name, errorMessage);

  bool ret = basedsl->dial(userName, password, deviceName, errorMessage);
  qDebug() << "failed: " << errorMessage;
  if (ret) {
    if (isRedial)
      emit redialFinished(true);
    else
      emit dialFinished(true);
    dialSucceed = true;
    while (1) {
      if (stopNow) {
        // Log::write("disconnect naturally\n");
        // Log::write(QString::number(ret)+ " " +
        // QString::number(stop_now)+"\n");
        basedsl->hangUp();
        // emit hangedUp(true);
        isDisconnectNaturally = true;
        break;
      }
      if (basedsl->isDisconnected()) {
        qDebug() << "Network Disconnected";
        isDisconnectNaturally = false;
        // emit hangedUp(false);
        break;
      }
      this->sleep(1);
    }
  } else {
    dialSucceed = false;
    if (isRedial)
      emit redialFinished(false);
    else
      emit dialFinished(false);
  }
  qDebug() << "break from pppoe";
}

QString PPPoE::getIpAddress() {
  auto interfaceList = QNetworkInterface::allInterfaces();

<<<<<<< HEAD
    for (auto const &interface: interfaceList) {
        if (interface.type() == QNetworkInterface::InterfaceType::Ppp) {
            if (interface.flags().testFlag(QNetworkInterface::IsPointToPoint)
                && interface.flags().testFlag(QNetworkInterface::IsRunning))
            qDebug() << interface.name();
            qDebug() << interface.type();
            qDebug() << interface.hardwareAddress();
            auto addressEntries = interface.addressEntries();
            qDebug() << addressEntries.size();
            auto address = addressEntries.front();
            qDebug() << address.ip();
            auto ipv4Address = address.ip().toIPv4Address();
            return QHostAddress{ipv4Address}.toString();
        }
=======
  for (auto const &interface : interfaceList) {
    if (interface.type() == QNetworkInterface::InterfaceType::Ppp) {
      if (interface.flags().testFlag(QNetworkInterface::IsPointToPoint) &&
          interface.flags().testFlag(QNetworkInterface::IsRunning))
        qDebug() << interface.name();
      qDebug() << interface.type();
      qDebug() << interface.hardwareAddress();
      auto addressEntries = interface.addressEntries();
      qDebug() << addressEntries.size();
      auto address = addressEntries.front();
      qDebug() << address.ip();
      return address.ip().toString();
>>>>>>> 84f480745fc12a6fa2ebf43b47bc0646daea3516
    }
  }
  return {};
}

QString PPPoE::getUserName() { return this->userName; }

bool PPPoE::dialRAS(QString const &entryName, QString const &userName,
                    QString const &password, QString const &deviceName) {
  qDebug() << "PPPoE::dialRAS"
           << "device_name" << deviceName;
  if (this->isRunning())
    return false;
  this->entryName = entryName;
  this->userName = userName;
  this->password = password;
  this->deviceName = deviceName;
  this->isRedial = false;
  this->stopNow = false;
  qDebug() << "userName" << userName;
  qDebug() << "password" << password;
  this->start();
  return true;
}
bool PPPoE::redialRAS() {
  if (this->isRunning())
    return false;
  isRedial = true;
  this->stopNow = false;
  // this->dialRAS(this->entryName,this->username,this->password);
  this->start();
  return true;
}

void PPPoE::hangUp() { stopNow = true; }

bool PPPoE::preparePhoneBook() { return false; }

void PPPoE::threadFinished() {
  if (dialSucceed) {
    dialSucceed = false;
    emit hangedUp(isDisconnectNaturally);
  }
}

QStringList PPPoE::getAvailableInterfaces() {
  return basedsl->getAvailableInterfaces();
}

bool PPPoE::isDisconnect() { return basedsl->isDisconnected(); }

QString PPPoE::getDeviceName() const { return this->deviceName; }
