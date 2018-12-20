#ifndef LINUXBASEDSL_HPP
#define LINUXBASEDSL_HPP

#include <NetworkManager/NetworkManager.h>
#include <QHostAddress>
#include <QtCore/QUuid>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusReply>
#include <arpa/inet.h>
#include <basedsl.hpp>
#include <unistd.h>
typedef QMap<QString, QMap<QString, QVariant>> Connection;
Q_DECLARE_METATYPE(Connection)
class LinuxBaseDsl final : public BaseDsl {
  Q_OBJECT
public:
  LinuxBaseDsl();
  ~LinuxBaseDsl() override = default;
  bool dial(const QString &username, const QString &password,
            const QString &device_name, QString &errorMessage) override;
  void hangUp() override;
  bool isDisconnected() override;
  QStringList getAvailableInterfaces() override;

private:
  QSharedPointer<QDBusInterface> networkmanager;
  QDBusObjectPath objectPath;
  QDBusObjectPath activeObjectPath;
};

#endif // LINUXBASEDSL_HPP
