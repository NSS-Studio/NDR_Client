#include "userinformation.h"

userInformation::userInformation(QObject *parent) :
    QObject(parent)
{
}
#ifdef Q_OS_WIN
void userInformation::getMac()
{






}
#endif


#ifdef Q_OS_LINUX
void userInformation::getMac(QString interface)
{

}
#endif
void userInformation::getUsername(QString username)
{

}
void userInformation::getPassword(QString password)
{

}
void userInformation::getIp()
{

}
void userInformation::getLoginTime()
{

}
void userInformation::getVersion()
{

}
