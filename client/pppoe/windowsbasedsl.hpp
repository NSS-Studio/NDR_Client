#ifndef WINDOWSBASEDSL_HPP
#define WINDOWSBASEDSL_HPP
#include "basedsl.hpp"
#define RASNP_Ipv6 0x00000008
#include <ras.h>
#include <raserror.h>
#include <windows.h>
class WindowsBaseDsl final: public BaseDsl
{
public:
    WindowsBaseDsl();
    ~WindowsBaseDsl();
    bool dial(const QString &username, const QString &password,const QString &device_name, QString &errorMessage);
    void hangUp();
    bool isDisconnected();
    QStringList getAvailableInterfaces();
private:
    HRASCONN hRasConn;
    bool preparePhoneBookEntry(QString const entryName,
                               QString &errorMessage);
};

#endif // WINDOWSBASEDSL_HPP
