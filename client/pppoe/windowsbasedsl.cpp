#include "windowsbasedsl.hpp"
#include <utils.hpp>

WindowsBaseDsl::WindowsBaseDsl()
    :hRasConn{NULL}
{

}

WindowsBaseDsl::~WindowsBaseDsl() {

}

bool WindowsBaseDsl::dial(QString const &username,
          QString const &password,
          QString const &device_name,
          QString &errorMessage) {
    Q_UNUSED(device_name);
    DWORD ret;
    RASDIALPARAMSW params;

    if(!preparePhoneBookEntry(name,errorMessage))
    {
        errorMessage += "\n" + tr("请尝试手动创建名为%1的适配器").arg(name);
        errorMessage = tr("拨号连接适配器初始化失败，%1").arg(errorMessage);

        return false;
    }
    memset(&params, 0, sizeof(params));
    params.dwSize = sizeof(RASDIALPARAMSW);
    name.toWCharArray(params.szEntryName);
    lstrcpyW(params.szPhoneNumber, L"");
    lstrcpyW(params.szCallbackNumber, L"");
    username.toWCharArray(params.szUserName);
    password.toWCharArray(params.szPassword);
    lstrcpyW(params.szDomain,L"");
    ret=RasDialW(nullptr, nullptr, &params, static_cast<DWORD>(NULL), nullptr, &hRasConn);
    qDebug() << "pppoe ret" << ret;
    if(ret!=0)
    {
         /*****************/
        if(ret==ERROR_FROM_DEVICE/*防止下次756*/
            || ret==ERROR_NO_ANSWER )
        {
            RasHangUpW(hRasConn);
        }
        /*****************/
        wchar_t szError[1024];
        if(RasGetErrorStringW(ret,szError,sizeof(szError))==ERROR_SUCCESS)
        {
            errorMessage.setUtf16((const unsigned short*)szError,lstrlenW(szError));
        }else
        {
            errorMessage = tr("未知错误");
        }

        /* 将Error Code下移，强迫用户看常用错误帮助文档*/
        errorMessage = QString::number(ret,10) + " " + errorMessage;
        switch(ret)
        {
        case 651:
            errorMessage = tr(
                "1.检查网络线路是否通畅（尝试使用其他可正常拨号PC连接该网口拨号）"
                "\n2.请确保交换机及其连线完好."
                "\n3.请确保网线已插好."
                "\n若如上做问题仍未解决，请上报修网报修"
                "\n网址为: http://bx.neusoft.edu.cn/"
                "\n ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n") + errorMessage;
            break;
        case 691:
            errorMessage = tr(
                "1.请检查账号或密码以及套餐设置是否正确."
                "\n2.请检查套餐是否还未生效或已经过期."
                "\n3.请确保当前没有连接到其他Wifi网络."
                "\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n") + errorMessage;
            break;
        case 756:
            errorMessage = tr(
                "请尝试重新启动计算机.\n"
                "\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n") + errorMessage;
            break;
        case 668:
            errorMessage = tr(
                "请重新尝试拨号.\n"
                "\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n") + errorMessage;
            break;
        case 1062:
            errorMessage = tr(
                "可能是一些拨号所依赖的服务无法启动所致\n"
                "\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n") + errorMessage;
            break;
        }
        hRasConn = NULL;
        return false;
    }
    return true;
}

void WindowsBaseDsl::hangUp() {
    RasHangUpW(hRasConn);
    hRasConn = NULL;
}

bool WindowsBaseDsl::isDisconnected() {
    RASCONNSTATUSW rcs;
    DWORD ret;
    rcs.dwSize = sizeof(RASCONNSTATUSW);
    ret = RasGetConnectStatusW(hRasConn,&rcs);
    return ret || rcs.rasconnstate == RASCS_Disconnected;
}

QStringList WindowsBaseDsl::getAvailableInterfaces() {
    return QStringList{};
}

bool WindowsBaseDsl::preparePhoneBookEntry(QString const entryName,
                                           QString &errorMessage) {
    DWORD dwBufferSize = sizeof(RASENTRYW) ;
    DWORD dwRet = 0 ;
    WCHAR szEntryName[1024];
    RASENTRYW rasEntry;
    memset( &rasEntry, 0, sizeof(RASENTRYW));
    szEntryName[0]=L'\0';
    rasEntry.dwSize = sizeof(RASENTRYW);
    entryName.toWCharArray(szEntryName);
    dwRet=RasGetEntryPropertiesW(nullptr, szEntryName , &rasEntry,  & dwBufferSize, nullptr, nullptr);
    qDebug() << "dwRet" << dwRet;
    if(ERROR_CANNOT_FIND_PHONEBOOK_ENTRY==dwRet)
    {
        qDebug() << "Phone book entry not found";

        DWORD cb = sizeof (RASENTRYW);
        rasEntry.dwSize = cb;
        rasEntry.dwfOptions = RASEO_RemoteDefaultGateway|
            //RASEO_PreviewPhoneNumber|
            //RASEO_PreviewUserPw|
            RASEO_ModemLights;

        lstrcpy(rasEntry.szDeviceType,RASDT_Modem);
        //TODO fill rasEntry.szDeviceName by RasEnumDevices ;
        {
            DWORD ret,dwSize,dwDevices;
            RASDEVINFOW arrDevInfo[64];
            dwSize = sizeof(arrDevInfo);
            arrDevInfo[0].dwSize = sizeof(RASDEVINFOW);
            ret = RasEnumDevicesW(arrDevInfo,&dwSize,&dwDevices);
            if(ret)
            {
                qDebug() << "RasEnumDevicesW() error" <<  ret;
                WCHAR szError[1024];
                errorMessage = tr("枚举RAS-CAPABLE设备时返回错误") + "\n";
                errorMessage += QString::number(dwRet,10);
                if(RasGetErrorStringW(dwRet,szError,sizeof(szError))==ERROR_SUCCESS)
                {
                    QString message;
                    message.setUtf16((const unsigned short*)szError,lstrlenW(szError));

                    errorMessage += " " + message;
                }else
                {
                    errorMessage += " " + tr("未知错误");
                }
                //return RasGetErrorStringW()
                return false;
            }else
            {
                qDebug() << "RasEnumDevicesW() succeed" <<  ret;
                BOOL bFound=FALSE;
                DWORD i=0;
                for(;i<dwDevices;i++)
                {
                    qDebug() << QString::fromUtf16((const ushort*)arrDevInfo[i].szDeviceName)
                                <<QString::fromUtf16((const ushort*)arrDevInfo[i].szDeviceType);
                    if(!lstrcmpW(arrDevInfo[i].szDeviceType,L"pppoe"))
                    {
                        bFound=TRUE;
                        break;
                    }
                }

                if(!bFound)
                {
                    qDebug() << "Device not found";
                    errorMessage = tr("没有可用的RAS-CAPABLE设备");
                    return false;
                }

                lstrcpy(rasEntry.szDeviceName,arrDevInfo[i].szDeviceName);
                qDebug() << "Device name" << arrDevInfo[i].szDeviceName;
            }
        }
        rasEntry.dwfNetProtocols  =  RASNP_Ip;
        rasEntry.dwFramingProtocol = RASFP_Ppp;
        rasEntry.dwfNetProtocols = RASNP_Ipv6 | RASNP_Ip;
        dwRet  =  RasSetEntryPropertiesW(nullptr,  szEntryName , &rasEntry, cb, nullptr,  0 );
        if(dwRet)
        {
            WCHAR szError[1024];
            if(RasGetErrorStringW(dwRet,szError,sizeof(szError))==ERROR_SUCCESS)
            {
                errorMessage.setUtf16((const unsigned short*)szError,lstrlenW(szError));
                errorMessage = QString::number(dwRet,10) + " " + errorMessage;
            }else
            {
                errorMessage = QString::number(dwRet,10) + " " + tr("未知错误");
            }
            return false;
        }else
            return true;

    }
    return true;
}
