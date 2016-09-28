#include "interfaceinfo.h"

#if (defined Q_OS_LINUX||defined Q_OS_MAC)
InterfaceInfo::InterfaceInfo(QString lowerInterface, QString upperInterface, QObject *parent) :
    QObject(parent)
{
    this->ipAddress = "";
    this->macAddress = "";
    this->interfaceName = "";

    this->error = true;
    if (0 == getInterfaceInfo(lowerInterface, upperInterface))
    {
        this->error = false;
    }else
    {
        this->error = true;
    }

}
#endif

#ifdef Q_OS_WIN
InterfaceInfo::InterfaceInfo(QString adapterName,  QObject *parent) :
    QObject(parent)
{
    this->ipAddress = "";
    this->macAddress = "";
    this->interfaceName = "";

    this->error = true;
    if (0 == getInterfaceInfo(adapter))
    {
        this->error = false;
    }else
    {
        this->error = true;
    }

}
#endif

bool InterfaceInfo::isError()                       //处理构造函数错误
{
    return this->error;
}

QString InterfaceInfo::getMac()
{
    return this->macAddress;
}
QString InterfaceInfo::getIp()
{
    return this->ipAddress;
}

#ifdef Q_OS_WIN
void userInformation::getInterfaceInfo(QString adapterName))
{
    _IP_ADAPTER_ADDRESSES *adapterAddresses = (IP_ADAPTER_ADDRESSES*) malloc(sizeof(IP_ADAPTER_ADDRESSES));
    PULONG sizePointer = new ULONG;
    *sizePointer = 0;
    int isError = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_TUNNEL_BINDINGORDER, NULL, adapterAddresses, sizePointer);
    if (ERROR_BUFFER_OVERFLOW== isError)
    {
        free(adapterAddresses);
        _IP_ADAPTER_ADDRESSES *adapterAddresses = (IP_ADAPTER_ADDRESSES*) malloc(*sizPointer);
        int isError = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_TUNNEL_BINDINGORDER, NULL, adapterAddresses, sizePointer);
    }
    if (NO_ERROR== isError)
    {
    }
    free(adapterAddresses);
}

#endif


#if (defined Q_OS_LINUX||defined Q_OS_MAC)
int InterfaceInfo::getInterfaceInfo(QString lowerInterface, QString upperInterface)
{
    int i;
    char *cmacAddress = (char*)malloc(sizeof(char) * 2);
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (-1 == fd)
    {
        free(cmacAddress);                                      //释放内存
        return -1;
    }
    strcpy(s.ifr_name, upperInterface.toAscii());
    //通过网卡名称获取IP地址
    if (0 == ioctl(fd, SIOCGIFADDR, &s))
    {
        this->ipAddress = inet_ntoa(((struct sockaddr_in *)&s.ifr_addr)->sin_addr);
    }
    strcpy(s.ifr_name, lowerInterface.toAscii());
    //通过网卡名称获取MAC地址
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
    close(fd);
    return 0;
}
#endif

