#include "interfaceinfo.h"

InterfaceInfo::InterfaceInfo(QString lowerInterface, QString upperInterface, QObject *parent) :
    QObject(parent)
{
    this->ipAddress = "";
    this->macAddress = "";
    this->lowerInterface = lowerInterface;
    this->upperInterface = upperInterface;
}

QString InterfaceInfo::getMac111()
{
    return this->macAddress;
}

QString InterfaceInfo::getIp()
{
    return this->ipAddress;
}

#ifdef Q_OS_WIN
int InterfaceInfo::getInterfaceInfo(QString lowerInterface, QString upperInterface)
{
    int count = 0;
    int isError = 0;
    IP_ADAPTER_ADDRESSES *adapterAddresses = NULL;
    ULONG sizePointer;
    sizePointer = WORKING_BUFFER_SIZE;
    short int Iterations = 0;
    do {
        adapterAddresses = (IP_ADAPTER_ADDRESSES*) malloc(sizePointer);
        if (adapterAddresses == NULL)
        {
            free(adapterAddresses);
            adapterAddresses = NULL;
            Iterations++;
            continue;
        }
        isError = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &sizePointer);
        if (ERROR_BUFFER_OVERFLOW == isError)
        {
            free(adapterAddresses);
            adapterAddresses = NULL;
            qDebug() << "isError!?";
        } else {
            break;
        }
        Iterations++;
    }
    while ((isError == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (isError == NO_ERROR)
    {
        char *cmacAddress = "";
        cmacAddress = (char*)malloc(1);
        QString qmacAddress = "";
        IP_ADAPTER_ADDRESSES *pAdapterAddresses;
        for (pAdapterAddresses = adapterAddresses; pAdapterAddresses != NULL; pAdapterAddresses = pAdapterAddresses->Next)
        {
            wchar_t *friendlyName;
            friendlyName = (wchar_t *)malloc(sizeof(256));
            lowerInterface.toWCharArray(friendlyName);
            friendlyName[lowerInterface.count()] = '\0';
            if (!wcscmp(pAdapterAddresses->FriendlyName, friendlyName))
            {
                for (int i = 0; i < 6; ++i)
                {
                    sprintf(cmacAddress, "%02x:", adapterAddresses->PhysicalAddress[i]);
                    qmacAddress += cmacAddress;
                }
            }
            count++;
        }
        qDebug() << "get adapter success!";
        return 0;
    }else
    {
        return 1;
    }
    free(adapterAddresses);                     //注意：此处没释放完！
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

