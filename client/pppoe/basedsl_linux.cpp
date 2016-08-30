#include "basedsl.h"
#include <QtCore/QDebug>

BaseDsl::BaseDsl(const QString &name, QObject *parent) :
	QObject(parent)
{
	qDBusRegisterMetaType<Connection>();
	this->name = name;
	networkmanager = new QDBusInterface(NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, QDBusConnection::systemBus());
}

bool BaseDsl::dial(const QString &username, const QString &password, const QString &device_name, QString &errorMessage)
{
	QString last_conn_path;

	// Create a new connection object
	Connection connection;

	QSettings conn_cfg(appHome + "/connection.cfg", QSettings::IniFormat);
	QString uuid = conn_cfg.value("DSL/UUID", QUuid::createUuid().toString().remove('{').remove('}')).toString();
/*
	QString device_name = conn_cfg.value("Device/Interface").toString();
	if(device_name.isEmpty()) {
		device_name = get_device_name_from_user("eth0", NULL);
		if(device_name.isEmpty()) {
			errorMessage = tr("未选择网卡");
			return false;
		}
		conn_cfg.setValue("Device/Interface", device_name);
	}
*/
	QDBusInterface nm_settings(NM_DBUS_SERVICE, NM_DBUS_PATH_SETTINGS, NM_DBUS_IFACE_SETTINGS, QDBusConnection::systemBus());
	QDBusReply<QDBusObjectPath> last_conn_reply = nm_settings.call("GetConnectionByUuid", uuid);
	if(last_conn_reply.isValid()) {
		qDebug() << last_conn_reply.value().path();
		//QDBusInterface last_conn_setting(NM_DBUS_SERVICE, last_conn_reply.value().path(), NM_DBUS_IFACE_SETTINGS_CONNECTION, QDBusConnection::systemBus());
		//QDBusMessage reply = last_conn_setting.call("Delete");
		//qDebug() << "type" << reply.type();
		//qDebug() << "error" << reply.errorMessage();
		last_conn_path = last_conn_reply.value().path();
	} else qDebug() << "!valid" << last_conn_reply.error().message();

	// Build up the 'connection' Setting
	connection["connection"]["uuid"] = uuid;
	connection["connection"]["id"] = name;
	connection["connection"]["type"] = "pppoe";
	connection["connection"]["autoconnect"] = false;

	connection["pppoe"]["username"] = username;
	connection["pppoe"]["password"] = password;
	connection["ppp"]["require-mppe"] = false;
	connection["802-3-ethernet"]["duplex"] = "full";
	connection["ipv4"]["method"] = "auto";
	connection["ipv6"]["method"] = "auto";

	QDBusReply<QDBusObjectPath> device_reply = networkmanager->call("GetDeviceByIpIface", device_name);
	if(!device_reply.isValid()) {
		errorMessage = device_reply.error().message();
		if(errorMessage == "No device found for the requested iface.") {
			errorMessage = (tr("找不到网卡 %1 或该网卡被占用")).arg(device_name);
		}
		return false;
	}
	qDebug() << device_reply.isValid();
	QDBusObjectPath device = device_reply.value();
	qDebug() << "device" << device.path();
	QDBusMessage result;
	if(last_conn_path.isEmpty()) {
		result = networkmanager->call("AddAndActivateConnection", QVariant::fromValue(connection),
			QVariant::fromValue(device), QVariant::fromValue(QDBusObjectPath("/")));
		conn_cfg.setValue("DSL/UUID", uuid);
		qDebug() << "type:" << result.type();
		if(result.type() == QDBusMessage::ErrorMessage) {
			errorMessage = result.errorMessage();
			return false;
		}
		QList<QVariant> list = result.arguments();

	/*	http://stackoverflow.com/questions/20206376/how-do-i-extract-the-returned-data-from-qdbusmessage-in-a-qt-dbus-call
		qDebug() << "list" << list;
		QVariant first = list.at(0);
		qDebug() << "first" << first;
		QDBusVariant dbvFirst = first.value<QDBusVariant>();
		QVariant vFirst = dbvFirst.variant();
		qDebug() << "vFirst" << vFirst;
		const QDBusArgument &dbusArgs = vFirst.value<QDBusArgument>();
		qDebug() << "QDBusArgument current type is" << dbusArgs.currentType();
		dbusArgs.beginArray();
		dbusArgs >> objectPath;
		dbusArgs >> activeObjectPath;
		dbusArgs.endArray();
	*/
		objectPath = QDBusObjectPath(list[0].value<QDBusObjectPath>());
		activeObjectPath = QDBusObjectPath(list[1].value<QDBusObjectPath>());
		qDebug() << "active" << activeObjectPath.path();
	} else {
		QDBusInterface last_conn_setting(NM_DBUS_SERVICE, last_conn_path, NM_DBUS_IFACE_SETTINGS_CONNECTION, QDBusConnection::systemBus());
		result = last_conn_setting.call("Update", QVariant::fromValue(connection));
		if(result.type() == QDBusMessage::ErrorMessage) {
			errorMessage = result.errorMessage();
			return false;
		}
		QDBusReply<QDBusObjectPath> reply = networkmanager->call("ActivateConnection", QVariant::fromValue(QDBusObjectPath(last_conn_path)), QVariant::fromValue(device), QVariant::fromValue(QDBusObjectPath("/")));
		if(!reply.isValid()) {
			errorMessage = reply.error().message();
			return false;
		}
		objectPath = QDBusObjectPath(last_conn_path);
		activeObjectPath = reply.value();
	}


	QDBusInterface dbus_prop(NM_DBUS_SERVICE, activeObjectPath.path(), "org.freedesktop.DBus.Properties", QDBusConnection::systemBus());
	unsigned int state;
	do {
		usleep(200 * 1000);
		QDBusReply < QDBusVariant > reply = dbus_prop.call("Get", NM_DBUS_INTERFACE_ACTIVE_CONNECTION, "State");
		if(!reply.isValid()) {
            //errorMessage = reply.error().message();
            errorMessage = tr("您可使用以下命令行来查看错误信息") + "\n$ tail -n 200 /var/log/syslog | sed -rn '/pppd\\[[0-9]+\\]: .+/p' | sed -r 's/\\b\\w+\\b pppd\\[[0-9]+\\]://'";
			return false;
		}
		state = reply.value().variant().toUInt();
	} while(state == NM_ACTIVE_CONNECTION_STATE_ACTIVATING);
	return state == NM_ACTIVE_CONNECTION_STATE_ACTIVATED;
}

void BaseDsl::hangUp()
{
	networkmanager->call("DeactivateConnection", QVariant::fromValue(activeObjectPath));
}

bool BaseDsl::isDisconnected()
{
	//QDBusInterface active_connection(NM_DBUS_SERVICE, activeObjectPath.path(), NM_DBUS_INTERFACE_ACTIVE_CONNECTION, QDBusConnection::systemBus());
	QDBusInterface dbus_prop(NM_DBUS_SERVICE, activeObjectPath.path(), "org.freedesktop.DBus.Properties", QDBusConnection::systemBus());
	QDBusReply < QDBusVariant > reply = dbus_prop.call("Get", NM_DBUS_INTERFACE_ACTIVE_CONNECTION, "State");
	//qDebug() << "State Valid ?" << reply.isValid();
	if(!reply.isValid()) {
		qDebug() << reply.error().message();
		return true;
	}
	unsigned int state = reply.value().variant().toUInt();
	//qDebug() << state;
	return state != NM_ACTIVE_CONNECTION_STATE_ACTIVATING &&
		state != NM_ACTIVE_CONNECTION_STATE_ACTIVATED;
}

QString BaseDsl::getIpAddress()
{
	QDBusInterface active_prop(NM_DBUS_SERVICE, activeObjectPath.path(), "org.freedesktop.DBus.Properties", QDBusConnection::systemBus());
	QDBusReply<QDBusVariant> device_reply = active_prop.call("Get", NM_DBUS_INTERFACE_ACTIVE_CONNECTION, "Devices");
	if(!device_reply.isValid()) {
		return device_reply.error().message();
	}
	QDBusArgument args = device_reply.value().variant().value<QDBusArgument>();
	args.beginArray();
	QDBusObjectPath device_object_path;
	args >> device_object_path;
	//qDebug() << device_object_path.path();
	args.endArray();
	QDBusInterface device_prop(NM_DBUS_SERVICE, device_object_path.path(), "org.freedesktop.DBus.Properties", QDBusConnection::systemBus());
	QDBusReply<QDBusVariant> ipaddr_reply = device_prop.call("Get", NM_DBUS_INTERFACE_DEVICE, "Ip4Address");
	if(!ipaddr_reply.isValid()) {
		return ipaddr_reply.error().message();
	}
	unsigned int ipaddr = ipaddr_reply.value().variant().toUInt();
	qDebug("ipaddr = %u", ipaddr);
	return QHostAddress(htonl(ipaddr)).toString();
}

BaseDsl::~BaseDsl()
{
	delete networkmanager;
}

QStringList BaseDsl::get_available_interfaces() {
	QDBusInterface networkmanager(NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, QDBusConnection::systemBus());
	QDBusMessage result = networkmanager.call("GetDevices");
	qDebug() << result;
	if(result.type() == QDBusMessage::ErrorMessage) return QStringList();
	QStringList r;
	QDBusArgument args = result.arguments()[0].value<QDBusArgument>();
	args.beginArray();
	while(!args.atEnd()) {
		QDBusObjectPath path;
		args >> path;
		//qDebug() << path.path();
		QDBusInterface device_prop(NM_DBUS_SERVICE, path.path(), "org.freedesktop.DBus.Properties", QDBusConnection::systemBus());
		QDBusReply<QDBusVariant> type_reply = device_prop.call("Get", NM_DBUS_INTERFACE_DEVICE, "DeviceType");
		if(!type_reply.isValid()) continue;
		if(type_reply.value().variant().toUInt() == NM_DEVICE_TYPE_ETHERNET) {
			QDBusReply<QDBusVariant> interface_reply = device_prop.call("Get", NM_DBUS_INTERFACE_DEVICE, "Interface");
			if(!interface_reply.isValid()) continue;
			r << interface_reply.value().variant().toString();
		}
	}
	return r;
}
