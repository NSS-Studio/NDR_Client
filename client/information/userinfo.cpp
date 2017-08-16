#include "userinfo.h"

UserInfo::UserInfo(QString username, QString password, QObject *) :
    InterfaceInfo()
{
    this->username = username;
    this->password = password;

}

QString UserInfo::getUsername()
{
    return this->username;
}

QString UserInfo::getPassword()
{
    return this->password;
}

QString UserInfo::getVersion()
{
    QString versionMajor;
    versionMajor = QString::number(VERSION_MAJOR);
    QString versionMinor;
    versionMinor = QString::number(VERSION_MINOR);
    this->version = versionMajor + "." + versionMinor;
    return this->version;
}
