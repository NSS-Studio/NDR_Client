#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QObject>
#include <QNetworkInterface>

class userInformation : public QObject
{
    Q_OBJECT
public:
    explicit userInformation(QObject *parent = 0);
    void getMac();
    void getUsername(QString username);
    void getPassword(QString password);
    void getIp();
    void getLoginTime();
    void getVersion();
    QString ipAddress, macAddress, username, password, loginTime, version;

signals:

public slots:

};

#endif // USERINFORMATION_H
