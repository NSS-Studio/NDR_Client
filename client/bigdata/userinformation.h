#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QObject>
#include <QNetworkInterface>
#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
#define USRINFO_NO_ERROR 0

class userInformation : public QObject
{
    Q_OBJECT
public:
    explicit userInformation(QString interface, QObject *parent = 0);
    explicit userInformation(QString username, QString password, QString interface, QObject *parent);

    QString getUsername();
    QString getPassword();
    QString getIp();
    QString getMac();
    QString getInterfaceName();
//    QString getLoginTime();
    QString getVersion();
    bool isError();

signals:

public slots:

private:
    int getInterfaceInfo(QString interface);
    QString ipAddress, macAddress, username, password, loginTime, version, interfaceName;
    bool error;
};

#endif // USERINFORMATION_H
