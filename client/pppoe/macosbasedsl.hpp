#ifndef MACOSBASEDSL_HPP
#define MACOSBASEDSL_HPP
#include "basedsl.hpp"
#include <SystemConfiguration/SCSchemaDefinitions.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/types.h>
class MacOsBaseDsl final: public BaseDsl
{
public:
    MacOsBaseDsl();
    ~MacOsBaseDsl();
    bool dial(const QString &username, const QString &password,const QString &device_name, QString &errorMessage);
    void hangUp();
    bool isDisconnected();
    QStringList getAvailableInterfaces();
private:
    SCNetworkConnectionRef connection = nullptr;
    SCNetworkServiceRef ppp_service = nullptr;
    SCPreferencesRef preferences = nullptr;
    SCNetworkInterfaceRef ppp_interface = nullptr;
};

#endif // MACOSBASEDSL_HPP
