#ifndef BASEDSL_H
#define BASEDSL_H

#include "common.h"
#include <QObject>
#if defined(Q_OS_WIN32)

#define RASNP_Ipv6  0x00000008
#include <windows.h>
#include <ras.h>
#include <raserror.h>
#elif defined Q_OS_LINUX || defined Q_OS_SOLARIS
#include <NetworkManager/NetworkManager.h>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusObjectPath>
#include <QHostAddress>
#include <QtCore/QUuid>
#include <unistd.h>
#include <arpa/inet.h>
typedef QMap<QString, QMap<QString, QVariant> > Connection;
Q_DECLARE_METATYPE(Connection)
#elif defined Q_OS_MAC
#include <SystemConfiguration/SystemConfiguration.h>
#include <SystemConfiguration/SCSchemaDefinitions.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <QHostAddress>
#endif
#include <QtCore/QSettings>
/*
#include <QtGui/QInputDialog>
#include <QtGui/QLineEdit>
*/

class BaseDsl : public QObject
{
	Q_OBJECT
public:
	explicit BaseDsl(const QString &name, QObject *parent = 0);
	~BaseDsl();

    /**
     * @brief dial 开始PPPOE拨号
     * @param username 用户名
     * @param password 密码
     * @param device_name 网卡设备名(Windows 上会忽略此参数)
     * @param errorMessage 拨号失败错误消息
     * @return true返回成功
     */
#ifdef Q_OS_WIN
    bool dial(const QString &username, const QString &password, const QString , QString &errorMessage);
#elif defined Q_OS_LINUX || defined Q_OS_SOLARIS || defined Q_OS_MAC
    bool dial(const QString &username, const QString &password, const QString &device_name, QString &errorMessage);
#endif

    /**
     * @brief hangUp 挂断
     */
    void hangUp();

    /**
     * @brief isDisconnected 返回连接状态
     * @return true 为已连接 false为断开
     */
    bool isDisconnected();
    /**
     * @brief getIpAddress 获取IP地址
     * @return IP地址字符串
     */
	QString getIpAddress();

    /**
     * @brief get_available_interfaces 获取可用网卡列表
     * @return 网卡名字符串列表
     */
	static QStringList get_available_interfaces();

private:
	//QString get_device_name_from_user(const QString &, bool *);
	QString name;
#ifdef Q_OS_WIN
	HRASCONN hRasConn;
	bool preparePhoneBookEntry(QString entryName,QString errorMessage);
#elif defined Q_OS_LINUX || defined Q_OS_SOLARIS
	QDBusInterface *networkmanager;
	QDBusObjectPath objectPath;
	QDBusObjectPath activeObjectPath;
#elif defined Q_OS_MAC
	SCNetworkConnectionRef connection;
	SCNetworkServiceRef ppp_service;
	SCPreferencesRef preferences;
	SCNetworkInterfaceRef ppp_interface;
#endif
	
};

#endif // BASEDSL_H
