#ifndef AUTHVERIFY_HPP
#define AUTHVERIFY_HPP

#include <QJsonDocument>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QThread>
#include <QUdpSocket>
#include <QVariantMap>
class AuthVerify final : public QThread {
public:
  AuthVerify();
  ~AuthVerify() override = default;

  void authStop();

  virtual void run() override;

public slots:
  void authStart();

private:
  QVariantMap authInfomation;
  QSharedPointer<QUdpSocket> clientSocket;
};

#endif // AUTHVERIFY_HPP
