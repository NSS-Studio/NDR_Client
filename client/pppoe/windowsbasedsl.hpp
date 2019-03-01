#pragma once
#include "basedsl.hpp"
#define RASNP_Ipv6 0x00000008
#include <ras.h>
#include <raserror.h>
#include <windows.h>
class WindowsBaseDsl final : public BaseDsl {
  Q_OBJECT
public:
  WindowsBaseDsl();
  ~WindowsBaseDsl() override = default;
  bool dial(const QString &username, const QString &password,
            const QString &device_name, QString &errorMessage) override;
  void hangUp() override;
  bool isDisconnected() override;
  QStringList getAvailableInterfaces() override;

private:
  HRASCONN hRasConn;
  bool preparePhoneBookEntry(QString const entryName, QString &errorMessage);
};
