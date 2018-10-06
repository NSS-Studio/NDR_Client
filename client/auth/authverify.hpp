#ifndef AUTHVERIFY_HPP
#define AUTHVERIFY_HPP

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QVariantMap>
#include <QJsonDocument>
#include <QSharedPointer>
#include <QUdpSocket>
class AuthVerify : public QThread
{
public:
    AuthVerify();
    ~AuthVerify() Q_DECL_OVERRIDE;

    void authStop();

    virtual void run() Q_DECL_OVERRIDE;

public slots:
    void authStart();

private:
    QVariantMap authInfomation;
    QSharedPointer<QUdpSocket> clientSocket;
};

#endif // AUTHVERIFY_HPP
