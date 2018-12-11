#include "authverify.hpp"
#include "utils.hpp"
#include <QDebug>
AuthVerify::AuthVerify() {
  clientSocket.reset(new QUdpSocket{});
  clientSocket->bind();
}

void AuthVerify::authStop() {
  qDebug() << "Set stop object = true And ThreadID: "
           << QThread::currentThreadId();
  requestInterruption();
}

void AuthVerify::authStart() { this->start(); }

void AuthVerify::run() {
  qDebug() << " AuthVerify run";
  QHostAddress const ndrPlugServer{NDR_PLUG_SERVER};
  quint16 const ndrPlugPort{NDR_PLUG_PORT};
  while (true) {

    QJsonDocument jsonInfomation =
        QJsonDocument::fromVariant(this->authInfomation);
    clientSocket->writeDatagram(jsonInfomation.toBinaryData(), ndrPlugServer,
                                ndrPlugPort);

    if (!isInterruptionRequested()) {
      break;
    }
  }
}
