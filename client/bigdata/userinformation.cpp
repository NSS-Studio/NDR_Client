#include "userinformation.h"

userInformation::userInformation(QString interface, QObject *parent) :
    QObject(parent)
{
    this->ipAddress = "";
    this->macAddress = "";
    this->username = "";
    this->password = "";
    this->interfaceName = "";
    this->version = "";

    this->error = true;
    if (0 == getInterfaceInfo(interface))
    {
        this->error = false;
    }else
    {
        this->error = true;
    }

}

userInformation::userInformation(QString username, QString password, QString interface, QObject *parent) :
    QObject(parent)
{
    this->error = true;
    if (0 == getInterfaceInfo(interface))
    {
        this->error = false;
    }else
    {
        this->error = true;
    }
    this->username = username;
    this->password = password;
}

bool userInformation::isError()                       //处理构造函数错误
{
    return this->error;
}

QString userInformation::getMac()
{
    return this->macAddress;
}

QString userInformation::getInterfaceName()
{
    return this->interfaceName;
}

QString userInformation::getUsername()
{
    return this->username;
}
QString userInformation::getPassword()
{
    return this->password;
}
QString userInformation::getIp()
{
    return this->ipAddress;
}
//QString userInformation::getLoginTime()
//{
//    return this->loginTime;
//}
QString userInformation::getVersion()
{
    QString versionMajor;
    versionMajor = QString::number(VERSION_MAJOR);
    QString versionMinor;
    versionMinor = QString::number(VERSION_MINOR);
    this->version = versionMajor + "." + versionMinor;
    return this->version;
}

#ifdef Q_OS_WIN
void userInformation::getInterfaceInfo()
{

}

#endif


#ifdef Q_OS_LINUX
int userInformation::getInterfaceInfo(QString interface)
{
    struct sockaddr_in *sin = NULL;
    struct ifaddrs *ifa = NULL, *ifList;
//获取IP地址和网卡名称
    if (getifaddrs(&ifList) < 0)
        return -1;
    for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr->sa_family == AF_INET && ifa->ifa_name == interface)
        {
            sin = (struct sockaddr_in *)ifa->ifa_addr;
            this->ipAddress = inet_ntoa(sin->sin_addr);
            this->interfaceName =  ifa->ifa_name;
        }
    }
//获取MAC地址
    int i;
    char *cmacAddress = (char*)malloc(sizeof(char) * 2);
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, interface.toAscii());
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
    {

        QString qmacAddress = "";
        for (i = 0; i < 5; ++i)
        {
            sprintf(cmacAddress, "%02x:", (unsigned char)s.ifr_addr.sa_data[i]);
            qmacAddress += cmacAddress;
        }
        sprintf(cmacAddress, "%02x", (unsigned char)s.ifr_addr.sa_data[i]);
        qmacAddress += cmacAddress;
        this->macAddress = qmacAddress;
    }
//释放指针
    free(cmacAddress);
    freeifaddrs(ifList);
    return 0;
}
#endif


#ifdef Q_OS_MAC
void userInformation::getInterfaceInfo()
{

}
#endif

