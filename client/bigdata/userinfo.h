#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <interfaceinfo.h>
#include <common.h>

class UserInfo : public InterfaceInfo
{
    Q_OBJECT
public:
    explicit UserInfo(QString username, QString password, QString lowerInterface, QString upperInterface, QObject *parent = 0 );
    QString getVersion();
    QString getUsername();
    QString getPassword();
private:
    QString version, password, username;
signals:

public slots:

};

#endif // USERINFO_H
