#ifndef MACOSBASEDSL_HPP
#define MACOSBASEDSL_HPP
#include "basedsl.hpp"
#include <SystemConfiguration/SCSchemaDefinitions.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/types.h>
class MacOsBaseDsl final : public BaseDsl {
  Q_OBJECT
public:
  MacOsBaseDsl() = default;
  ~MacOsBaseDsl() override = default;
  bool dial(QString const &username, QString const &password,
            QString const &device_name, QString &errorMessage) override;
  void hangUp() override;
  bool isDisconnected() override;
  QStringList getAvailableInterfaces() override;

private:
  SCNetworkConnectionRef connection = nullptr;
  SCNetworkServiceRef ppp_service = nullptr;
  SCPreferencesRef preferences = nullptr;
  SCNetworkInterfaceRef ppp_interface = nullptr;
};

#endif // MACOSBASEDSL_HPP
