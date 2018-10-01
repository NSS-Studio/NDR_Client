#ifndef LINUXBASEDSL_HPP
#define LINUXBASEDSL_HPP

#include <basedsl.hpp>
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
#include <unistd.h>
typedef QMap<QString, QMap<QString, QVariant>> Connection;
Q_DECLARE_METATYPE(Connection)
class LinuxBaseDsl : public BaseDsl
{
public:
    LinuxBaseDsl();
    ~LinuxBaseDsl();
    bool dial(const QString &username,
              const QString &password,
              const QString &device_name,
              QString &errorMessage);
    void hangUp();
    bool isDisconnected();
    QString getIpAddress();
    QStringList getAvailableInterfaces();

private:
    QSharedPointer<QDBusInterface> networkmanager;
    QDBusObjectPath objectPath;
    QDBusObjectPath activeObjectPath;
};

#endif // LINUXBASEDSL_HPP
