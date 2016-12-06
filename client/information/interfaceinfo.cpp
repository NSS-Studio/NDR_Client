#include "interfaceinfo.h"

InterfaceInfo::InterfaceInfo( QObject *parent) :
    QObject(parent)
{
    this->ipAddressAvailable = false;
    this->macAddressAvailable = false;

    this->ipAddress = "";
    this->macAddress = "";
    this->lowerInterface = "";
    this->upperInterface = "";
}


//注意！此函数返回分两种情况：
//1.返回值为一个正常的ip
//2.返回值为NULL，此类情况为获取信息时出现错误造成的
QString *InterfaceInfo::getHwAddress()                               //获取硬件地址
{
    QString *pmacAddr = new QString(this->macAddress);
    if (this->macAddressAvailable)
        return pmacAddr;
    else
        return NULL;

}


//注意！此函数返回分三种情况：
//1.返回值为一个正常的ip
//2.返回值为NULL，此类情况为获取信息时出现错误造成的
//3.返回值为空，此类情况为，获取过程正常但此网卡无具体分配的ip地址
QString *InterfaceInfo::getIpAddress()                              //获取ip地址
{
    QString *pIpAddr = new QString(this->ipAddress);
    if (this->ipAddressAvailable)
        return pIpAddr;
    else
        return NULL;
}

#ifdef Q_OS_WIN
void InterfaceInfo::getInterfaceInfo(const QString & lowerInterface, QString)
{
    this->macAddressAvailable = false;
    this->ipAddressAvailable = false;
    DWORD ipIfIndex = 0;
    int count = 0;
    int isError = 0;
    IP_ADAPTER_ADDRESSES *adapterAddresses = NULL;
    ULONG sizePointer;
    sizePointer = WORKING_BUFFER_SIZE;
    short int Iterations = 0;
    do {
        adapterAddresses = (IP_ADAPTER_ADDRESSES*) malloc(sizePointer);
        if (adapterAddresses == NULL)                                   //空间分配失败
        {
            FREE(adapterAddresses);
            adapterAddresses = NULL;
            Iterations++;
            continue;
        }
        isError = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, adapterAddresses, &sizePointer);
        if (ERROR_BUFFER_OVERFLOW == isError)
        {
            FREE(adapterAddresses);
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
        char *cmacAddress;
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
                //mac地址获取
                for (int i = 0; i < 6; ++i)
                {
                    sprintf(cmacAddress, "%02x", pAdapterAddresses->PhysicalAddress[i]);
                    qmacAddress += cmacAddress;
                }
                qDebug() << "mac address is:" << qmacAddress;
                this->macAddress = qmacAddress;
                this->macAddressAvailable = true;
                ipIfIndex = pAdapterAddresses->IfIndex;                //获取ip地址的index
                qDebug() << "ipIfIndex" << ipIfIndex;

                //释放指针
                free(cmacAddress);
                free(friendlyName);
                break;
            }
            count++;
            free(cmacAddress);
            free(friendlyName);
        }

        if (!this->macAddressAvailable == true)
        {
            return;
        }

        //ip地址获取
        IN_ADDR IPAddr;
        PMIB_IPADDRTABLE pIPAddrTable = NULL;
        DWORD dwRetVal = 0;
        DWORD dwSize = 0;
        pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(sizeof (MIB_IPADDRTABLE));
        if (pIPAddrTable)
        {
            // Make an initial call to GetIpAddrTable to get the
            // necessary size into the dwSize variable
            if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
            {
                FREE(pIPAddrTable);
                pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(dwSize);
                qDebug () << "ERROR_INSUFFICIENT_BUFFER";
            }
            if (pIPAddrTable == NULL) {
                qDebug() << "Memory allocation failed for GetIpAddrTable\n";
                return;
            }
        }else if (pIPAddrTable == NULL)                             //空间分配失败
        {
            goto failed;
        }
        if ( (dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 )) != NO_ERROR )
        {
            goto failed;
        }
        qDebug() << "pipaddrtable->dwnumentries" << pIPAddrTable->dwNumEntries;
        for (int i=0; i < (int) pIPAddrTable->dwNumEntries; i++)
        {
            qDebug() << "dwindex" << pIPAddrTable->table[i].dwIndex;
            if (pIPAddrTable->table[i].dwIndex == ipIfIndex)
            {
                IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
                this->ipAddress = inet_ntoa(IPAddr);
                qDebug() << "the ip address with adapter is:" << this->ipAddress;
            }
        }

//success:
        this->ipAddressAvailable = true;
        qDebug() << "get adapter success!";
failed:
        FREE(pIPAddrTable);
        return;

    } else {
        return;
    }
}
#endif


#if (defined Q_OS_LINUX||defined Q_OS_MAC)
void InterfaceInfo:: getInterfaceInfo(const QString & lowerInterface, QString upperInterface)
{
    this->macAddressAvailable = false;
    this->ipAddressAvailable = false;
    int i;
    char *cmacAddress = (char*)malloc(sizeof(char) * 2);
    struct ifreq s;
    bzero( &s,sizeof(s));
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (-1 == fd)
    {
        free(cmacAddress);                                      //释放内存
        return ;
    }
    strcpy(s.ifr_name, upperInterface.toAscii());
    qDebug() << fd;
    qDebug() << s.ifr_name;
    //通过网卡名称获取IP地址
    int ipIoctlRet = ioctl(fd, SIOCGIFADDR, &s);
    qDebug()  << "the return value from ioctl(get ip address):" << ipIoctlRet;
    if (0 == ipIoctlRet)
    {
        this->ipAddress = inet_ntoa(((struct sockaddr_in *)&s.ifr_addr)->sin_addr);
        this->ipAddressAvailable = true;
    }
    strcpy(s.ifr_name, lowerInterface.toAscii());
    //通过网卡名称获取MAC地址
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
    {
        QString qmacAddress = "";
        for (i = 0; i < 6; ++i)
        {
            sprintf(cmacAddress, "%02x", (unsigned char)s.ifr_addr.sa_data[i]);
            qmacAddress += cmacAddress;
        }
        this->macAddress = qmacAddress;
        this->macAddressAvailable = true;
    }
//释放指针
    free(cmacAddress);
    close(fd);
}
#endif

