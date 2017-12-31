#include "basedsl.h"
#include <QtCore/QDebug>

BaseDsl::BaseDsl(const QString &name, QObject *parent) :
	QObject(parent)
{
	this->name = name;
	connection = nullptr;
	ppp_service = nullptr;
	preferences = nullptr;
	ppp_interface = nullptr;
}

bool BaseDsl::dial(const QString &username, const QString &password, const QString &device_name, QString &errorMsg)
{
	//qDebug() << "device_name" << device_name;
#define SET_ERROR_MSG(S) errorMsg=QString("%1: %2").arg(S).arg(SCErrorString(SCError()))

	if(ppp_service) {
		CFRelease(ppp_service);
		ppp_service = nullptr;
	}
	//QSettings conn_cfg(appHome + "/connection.cfg", QSettings::IniFormat);
/*
	QByteArray device_name = conn_cfg.value("Device/Interface").toByteArray();
	if(device_name.isEmpty()) {
		bool ok;
		device_name = get_device_name_from_user("en0", &ok).toUtf8();
		if(!ok) return false;
		qDebug() << "device_name" << device_name;
		if(device_name.isEmpty()) {
			errorMsg = tr("未选择网卡");
			return false;
		}
		conn_cfg.setValue("Device/Interface", device_name);
	}
*/

	QByteArray username_ba = username.toUtf8();
	QByteArray password_ba = password.toUtf8();
	//qDebug() << username_ba << password_ba;
	QByteArray name_ba = name.toUtf8();
	CFStringRef name_cf = CFStringCreateWithCString(nullptr, name_ba.data(), kCFStringEncodingUTF8);
	CFStringRef ppp_keys[] = {
		kSCPropNetPPPAuthName,
		kSCPropNetPPPAuthPassword,
		kSCPropNetPPPDisconnectOnFastUserSwitch,
		kSCPropNetPPPDisconnectOnIdle,
		kSCPropNetPPPDisconnectOnLogout,
		kSCPropNetPPPDisconnectOnSleep,
		kSCPropNetPPPCommRemoteAddress,
		kSCPropNetPPPLogfile
	};
	CFTypeRef ppp_values[] = {
		CFStringCreateWithCString(nullptr, username_ba.data(), kCFStringEncodingUTF8),
		CFStringCreateWithCString(nullptr, password_ba.data(), kCFStringEncodingUTF8),
		kCFBooleanFalse,
		kCFBooleanFalse,
		kCFBooleanTrue,
		kCFBooleanTrue,
		name_cf,
		CFSTR("/var/log/ppp.log")
	};
	CFDictionaryRef ppp_dict = CFDictionaryCreate(nullptr, (const void **)ppp_keys, (const void **)ppp_values, 8, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

	CFStringRef interface_keys[] = {
		kSCPropNetInterfaceType,
		kSCPropNetInterfaceSubType,
		kSCPropNetInterfaceHardware
	};
	CFStringRef interface_values[] = {
		kSCValNetInterfaceTypePPP,
		kSCValNetInterfaceSubTypePPPoE,
		CFSTR("Ethernet")
	};
	CFDictionaryRef interface_dict = CFDictionaryCreate(nullptr, (const void **)interface_keys, (const void **)interface_values, 3, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFStringRef ip4_keys[] = {
		kSCPropNetIPv4ConfigMethod
	};
	CFStringRef ip4_values[] = {
		kSCValNetIPv4ConfigMethodPPP
	};
	CFDictionaryRef ip4_dict = CFDictionaryCreate(nullptr, (const void **)ip4_keys, (const void **)ip4_values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFStringRef ip6_keys[] = {
		kSCPropNetIPv6ConfigMethod
	};
	CFStringRef ip6_values[] = {
		kSCValNetIPv6ConfigMethodAutomatic
	};
	CFDictionaryRef ip6_dict = CFDictionaryCreate(nullptr, (const void **)ip6_keys, (const void **)ip6_values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

	CFStringRef keys[] = {
		kSCEntNetInterface,
		kSCEntNetIPv4,
		kSCEntNetIPv6,
		kSCEntNetPPP,
		kSCPropUserDefinedName
	};
	CFTypeRef values[] = {
		interface_dict,
		ip4_dict,
		ip6_dict,
		ppp_dict,
		name_cf
	};
	CFDictionaryRef dict = CFDictionaryCreate(nullptr, (const void **)keys, (const void **)values, 5, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

	CFRelease(interface_dict);
	CFRelease(ip4_dict);
	CFRelease(ip6_dict);

	//SCPreferencesRef preferences;
	if(!preferences) {
		AuthorizationFlags root_flags =
			kAuthorizationFlagDefaults              |
			kAuthorizationFlagExtendRights          |
			kAuthorizationFlagInteractionAllowed    |
			kAuthorizationFlagPreAuthorize;
		AuthorizationRef auth;
		if(AuthorizationCreate(nullptr, kAuthorizationEmptyEnvironment, root_flags, &auth) == noErr) {
			preferences = SCPreferencesCreateWithAuthorization(nullptr, CFSTR("NSS"), nullptr, auth);
		} else {
			preferences = SCPreferencesCreate(nullptr, CFSTR("NSS"), nullptr);
		}
		if(!preferences) {
			SET_ERROR_MSG("SCPreferencesCreate");
			return false;
		}
	}

	//SCNetworkServiceRef ppp_service = NULL;
	CFArrayRef services = SCNetworkServiceCopyAll(preferences);
	if(!services) {
		errorMsg = "No network services";
		return false;
	}
	SCNetworkInterfaceRef en_interface = nullptr;
	CFIndex i, service_count = CFArrayGetCount(services);
	for(i=0; i<service_count; i++) {
		SCNetworkServiceRef service = (SCNetworkServiceRef)CFArrayGetValueAtIndex(services, i);
		CFStringRef service_name = SCNetworkServiceGetName(service);
		SCNetworkInterfaceRef iface = SCNetworkServiceGetInterface(service);
		if(CFStringCompare(service_name, name_cf, 0) == kCFCompareEqualTo) {
			//ppp_service = service;
			SCNetworkServiceRemove(service);
			SCNetworkSetRef set = SCNetworkSetCopyCurrent(preferences);
			if(!set) {
				SET_ERROR_MSG("SCNetworkSetCopyCurrent");
				return false;
			}
			SCNetworkSetRemoveService(set, service);
			if(!SCPreferencesCommitChanges(preferences)) {
				SET_ERROR_MSG("SCPreferencesCommitChanges");
				return false;
			}
			if(!SCPreferencesApplyChanges(preferences)) {
				SET_ERROR_MSG("SCPreferencesApplyChanges");
				return false;
			}
			SCPreferencesSynchronize(preferences);
			sleep(1);
			//break;
		}

		if(SCNetworkInterfaceGetInterfaceType(iface) == kSCNetworkInterfaceTypeEthernet) {
			QString unknown_iface_prefix = trUtf8("未知网卡") + " ";
			if(device_name.startsWith(unknown_iface_prefix)) {
				QString unknown_device_name = device_name;
				unknown_device_name.remove(unknown_iface_prefix);
				if(i == unknown_device_name.toInt()) {
					en_interface = iface;
				}
			} else {
				QByteArray device_name_ba = device_name.toUtf8();
				CFStringRef device_name_cf = CFStringCreateWithCString(nullptr, device_name_ba.data(), kCFStringEncodingUTF8);
				CFStringRef bsd_name = SCNetworkInterfaceGetBSDName(iface);
				qDebug("%s: bsd_name: %s", __func__, bsd_name ? CFStringGetCStringPtr(bsd_name, kCFStringEncodingUTF8) : "(none)");
				if(CFStringCompare(bsd_name, device_name_cf, 0) == kCFCompareEqualTo) {
					en_interface = iface;
				}
			}
		}
	}
	if(!en_interface) {
		//errorMsg = "No ethernet interface found";
		errorMsg = trUtf8("找不到用于拨号的网卡 %1").arg(QString(device_name));
		//conn_cfg.setValue("Device/Interface", QByteArray());
		CFRelease(services);
		return false;
	}

	if(!SCPreferencesLock(preferences, 1)) {
		SET_ERROR_MSG("SCPreferencesLock");
		CFRelease(services);
		return false;
	}

#if 0
	if(ppp_service) {
		SCNetworkInterfaceRef ppp_interface = SCNetworkServiceGetInterface(ppp_service);
		if(!ppp_interface) {
			SET_ERROR_MSG("SCNetworkServiceGetInterface");
			return false;
		}
		if(!SCNetworkInterfaceSetConfiguration(ppp_interface, dict) || !SCNetworkInterfaceSetConfiguration(ppp_interface, ppp_dict)) {
			SET_ERROR_MSG("SCNetworkInterfaceSetConfiguration");
			return false;
		}
		sleep(2);
		/*
		if(!SCNetworkConnectionCopyUserPreferences(NULL, &service_id, &dict)) {
			SET_ERROR_MSG("SCNetworkConnectionCopyUserPreferences");
			return false;
		}*/
		ppp_service = SCNetworkServiceCreate(pref, ppp_interface);
		sleep(2);
	} else {
#endif
		/*SCNetworkInterfaceRef */ppp_interface = SCNetworkInterfaceCreateWithInterface(en_interface, kSCNetworkInterfaceTypePPP);
		CFRelease(services);
		if(!ppp_interface) {
			SET_ERROR_MSG("SCNetworkInterfaceCreateWithInterface");
			return false;
		}
		ppp_service = SCNetworkServiceCreate(preferences, ppp_interface);
		SCNetworkServiceSetName(ppp_service, name_cf);
		ppp_interface = SCNetworkServiceGetInterface(ppp_service);
		if(!SCNetworkInterfaceSetConfiguration(ppp_interface, dict) || !SCNetworkInterfaceSetConfiguration(ppp_interface, ppp_dict)) {
			SET_ERROR_MSG("SCNetworkInterfaceSetConfiguration");
			return false;
		}
		CFRelease(ppp_dict);

		if(!SCNetworkServiceEstablishDefaultConfiguration(ppp_service)) {
			SET_ERROR_MSG("SCNetworkServiceEstablishDefaultConfiguration");
			return false;
		}
		SCNetworkSetRef set = SCNetworkSetCopyCurrent(preferences);
		if(!set) {
			SET_ERROR_MSG("SCNetworkSetCopyCurrent");
			return false;
		}
		if(!SCNetworkSetAddService(set, ppp_service)) {
			SET_ERROR_MSG("SCNetworkSetAddService");
			return false;
		}
		if(!SCPreferencesCommitChanges(preferences)) {
			SET_ERROR_MSG("SCPreferencesCommitChanges");
			return false;
		}
		if(!SCPreferencesApplyChanges(preferences)) {
			SET_ERROR_MSG("SCPreferencesApplyChanges");
			return false;
		}
		SCPreferencesSynchronize(preferences);
//	}
	CFStringRef service_id = SCNetworkServiceGetServiceID(ppp_service);

	SCPreferencesUnlock(preferences);

	connection = SCNetworkConnectionCreateWithServiceID(nullptr, service_id, nullptr, nullptr);
	if(!connection) {
		SET_ERROR_MSG("SCNetworkConnectionCreateWithServiceID");
		return false;
	}

	SCNetworkConnectionStatus status = SCNetworkConnectionGetStatus(connection);
	if(status != kSCNetworkConnectionDisconnected) {
		errorMsg = trUtf8("期待目标连接的状态为 0，但是当前的状态为 %1").arg(status);
		return false;
	}

	if(!SCNetworkConnectionStart(connection, nullptr, 0)) {
		SET_ERROR_MSG("SCNetworkConnectionStart");
		return false;
	}

	do {
		usleep(200 * 1000);
		status = SCNetworkConnectionGetStatus(connection);
	} while(status == kSCNetworkConnectionConnecting);
	return status == kSCNetworkConnectionConnected;
#undef SET_ERROR_MSG
}

void BaseDsl::hangUp()
{
	if(!connection) return;
	SCNetworkConnectionStop(connection, 1);
	if(ppp_service) {
		SCNetworkServiceRemove(ppp_service);
		SCNetworkSetRef set = SCNetworkSetCopyCurrent(preferences);
		if(!set) return;
		SCNetworkSetRemoveService(set, ppp_service);
		if(!SCPreferencesCommitChanges(preferences)) return;
		if(!SCPreferencesApplyChanges(preferences)) return;
		SCPreferencesSynchronize(preferences);
		CFRelease(ppp_service);
		ppp_service = nullptr;
	}
	sleep(1);
}

bool BaseDsl::isDisconnected()
{
	if(!connection) return true;
	qDebug("connection = %p", connection);
	qDebug() << "status" << SCNetworkConnectionGetStatus(connection);
	return SCNetworkConnectionGetStatus(connection) == kSCNetworkConnectionDisconnected;
}

static in_addr_t get_address_by_interface(const char *iface) {
	struct ifaddrs *ifaddr = nullptr, *ifaddr_o;
	if(getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return (in_addr_t)-1;
	}
	ifaddr_o = ifaddr;
	do {
		if(ifaddr->ifa_addr &&
		ifaddr->ifa_addr->sa_family == AF_INET &&
		strcmp(ifaddr->ifa_name, iface) == 0) {
			in_addr_t r = ((struct sockaddr_in *)ifaddr->ifa_addr)->sin_addr.s_addr;
			freeifaddrs(ifaddr_o);
			return r;
		}
		ifaddr = ifaddr->ifa_next;
	} while(ifaddr);
	freeifaddrs(ifaddr_o);
	return (in_addr_t)-1;
}

QString BaseDsl::getIpAddress()
{
#if 0
	if(!ppp_interface) return "None";
	char buffer[8];
	CFStringRef ppp_bsd_name = SCNetworkInterfaceGetBSDName(ppp_interface);
	if(!ppp_bsd_name) return "ENOSYS";
	CFStringGetCString(ppp_bsd_name, buffer, sizeof buffer, kCFStringEncodingUTF8);
#else
	in_addr_t addr = get_address_by_interface("ppp0");
	if(addr == (in_addr_t)-1) return tr("未知");
	return QHostAddress(htonl(addr)).toString();
#endif
}

BaseDsl::~BaseDsl() {

}

QStringList BaseDsl::get_available_interfaces() {
	SCPreferencesRef preferences = SCPreferencesCreate(nullptr, CFSTR("NSS"), nullptr);
	CFArrayRef services = SCNetworkServiceCopyAll(preferences);
	CFRelease(preferences);
	if(!services) return QStringList();
	QStringList r;
	CFIndex i, service_count = CFArrayGetCount(services);
	for(i=0; i<service_count; i++) {
		SCNetworkServiceRef service = (SCNetworkServiceRef)CFArrayGetValueAtIndex(services, i);
		SCNetworkInterfaceRef iface = SCNetworkServiceGetInterface(service);
		CFStringRef name = SCNetworkInterfaceGetLocalizedDisplayName(iface);
		CFStringRef bsdn = SCNetworkInterfaceGetBSDName(iface);
		CFStringRef type = SCNetworkInterfaceGetInterfaceType(iface);
		qDebug("type = %p", type);
		qDebug("name: %s,	interface type: %s,	bsd name: %s",
			name ? CFStringGetCStringPtr(name, kCFStringEncodingUTF8) : "(none)",
			type ? CFStringGetCStringPtr(type, kCFStringEncodingUTF8) : "(none)",
			bsdn ? CFStringGetCStringPtr(bsdn, kCFStringEncodingUTF8) : "(none)");
		if(SCNetworkInterfaceGetInterfaceType(iface) == kSCNetworkInterfaceTypeEthernet) {
			CFStringRef bsd_name = SCNetworkInterfaceGetBSDName(iface);
			const char *p = CFStringGetCStringPtr(bsd_name, kCFStringEncodingUTF8);
			if(p) qDebug("bsd_name[0] = %hhu", (unsigned char)*p);
			qDebug("p == NULL\n");
			QString s;
			s = p ? QString::fromUtf8(p) : QString("%1 %2").arg(trUtf8("未知网卡")).arg(i);
			if(!r.contains(s)) r.append(s);
		}
	}
	CFRelease(services);
	return r;
}