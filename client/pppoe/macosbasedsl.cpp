#include <QHostAddress>
#include <macosbasedsl.hpp>
#include <utils.hpp>

static void setErrorMessage(QString &errorMessage, QString const &message) {
  errorMessage = QString("%1: %2").arg(message).arg(SCErrorString(SCError()));
}

bool MacOsBaseDsl::dial(QString const &username, QString const &password,
                        QString const &device_name, QString &errorMessage) {

  if (ppp_service) {
    CFRelease(ppp_service);
    ppp_service = nullptr;
  }

  CFStringRef name_cf = name.toCFString();
  CFStringRef ppp_keys[] = {kSCPropNetPPPAuthName,
                            kSCPropNetPPPAuthPassword,
                            kSCPropNetPPPDisconnectOnFastUserSwitch,
                            kSCPropNetPPPDisconnectOnIdle,
                            kSCPropNetPPPDisconnectOnLogout,
                            kSCPropNetPPPDisconnectOnSleep,
                            kSCPropNetPPPCommRemoteAddress,
                            kSCPropNetPPPLogfile};
  CFTypeRef ppp_values[] = {username.toCFString(),
                            password.toCFString(),
                            kCFBooleanFalse,
                            kCFBooleanFalse,
                            kCFBooleanTrue,
                            kCFBooleanTrue,
                            name_cf,
                            CFSTR("/var/log/ppp.log")};
  CFDictionaryRef ppp_dict = CFDictionaryCreate(
      nullptr, (const void **)ppp_keys, (const void **)ppp_values, 8,
      &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

  CFStringRef interface_keys[] = {kSCPropNetInterfaceType,
                                  kSCPropNetInterfaceSubType,
                                  kSCPropNetInterfaceHardware};
  CFStringRef interface_values[] = {kSCValNetInterfaceTypePPP,
                                    kSCValNetInterfaceSubTypePPPoE,
                                    CFSTR("Ethernet")};
  CFDictionaryRef interface_dict = CFDictionaryCreate(
      nullptr, (const void **)interface_keys, (const void **)interface_values,
      3, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  CFStringRef ip4_keys[] = {kSCPropNetIPv4ConfigMethod};
  CFStringRef ip4_values[] = {kSCValNetIPv4ConfigMethodPPP};
  CFDictionaryRef ip4_dict = CFDictionaryCreate(
      nullptr, (const void **)ip4_keys, (const void **)ip4_values, 1,
      &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  CFStringRef ip6_keys[] = {kSCPropNetIPv6ConfigMethod};
  CFStringRef ip6_values[] = {kSCValNetIPv6ConfigMethodAutomatic};
  CFDictionaryRef ip6_dict = CFDictionaryCreate(
      nullptr, (const void **)ip6_keys, (const void **)ip6_values, 1,
      &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

  CFStringRef keys[] = {kSCEntNetInterface, kSCEntNetIPv4, kSCEntNetIPv6,
                        kSCEntNetPPP, kSCPropUserDefinedName};
  CFTypeRef values[] = {interface_dict, ip4_dict, ip6_dict, ppp_dict, name_cf};
  CFDictionaryRef dict = CFDictionaryCreate(
      nullptr, (const void **)keys, (const void **)values, 5,
      &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

  CFRelease(interface_dict);
  CFRelease(ip4_dict);
  CFRelease(ip6_dict);

  // SCPreferencesRef preferences;
  if (!preferences) {
    AuthorizationFlags root_flags =
        kAuthorizationFlagDefaults | kAuthorizationFlagExtendRights |
        kAuthorizationFlagInteractionAllowed | kAuthorizationFlagPreAuthorize;
    AuthorizationRef auth;
    if (AuthorizationCreate(nullptr, kAuthorizationEmptyEnvironment, root_flags,
                            &auth) == noErr) {
      preferences = SCPreferencesCreateWithAuthorization(nullptr, CFSTR("NSS"),
                                                         nullptr, auth);
    } else {
      preferences = SCPreferencesCreate(nullptr, CFSTR("NSS"), nullptr);
    }
    if (!preferences) {
      setErrorMessage(errorMessage, "SCPreferencesCreate");
      return false;
    }
  }

  // SCNetworkServiceRef ppp_service = NULL;
  CFArrayRef services = SCNetworkServiceCopyAll(preferences);
  if (!services) {
    errorMessage = "No network services";
    return false;
  }
  SCNetworkInterfaceRef en_interface = nullptr;
  CFIndex service_count = CFArrayGetCount(services);
  for (CFIndex i = 0; i < service_count; i++) {
    SCNetworkServiceRef service =
        (SCNetworkServiceRef)CFArrayGetValueAtIndex(services, i);
    CFStringRef service_name = SCNetworkServiceGetName(service);
    SCNetworkInterfaceRef iface = SCNetworkServiceGetInterface(service);
    if (CFStringCompare(service_name, name_cf, 0) == kCFCompareEqualTo) {
      // ppp_service = service;
      SCNetworkServiceRemove(service);
      SCNetworkSetRef set = SCNetworkSetCopyCurrent(preferences);
      if (!set) {
        setErrorMessage(errorMessage, "SCNetworkSetCopyCurrent");
        return false;
      }
      SCNetworkSetRemoveService(set, service);
      if (!SCPreferencesCommitChanges(preferences)) {
        setErrorMessage(errorMessage, "SCPreferencesCommitChanges");
        return false;
      }
      if (!SCPreferencesApplyChanges(preferences)) {
        setErrorMessage(errorMessage, "SCPreferencesApplyChanges");
        return false;
      }
      SCPreferencesSynchronize(preferences);
      sleep(1);
      // break;
    }

    if (SCNetworkInterfaceGetInterfaceType(iface) ==
        kSCNetworkInterfaceTypeEthernet) {

      QString unknown_iface_prefix = tr("未知网卡") + " ";
      if (device_name.startsWith(unknown_iface_prefix)) {
        QString unknown_device_name = device_name;
        unknown_device_name.remove(unknown_iface_prefix);
        if (i == unknown_device_name.toInt()) {
          en_interface = iface;
        }
      } else {
        //! Associate base_dsl::get_available_interfaces member function
        //! need the same network interface name
        CFStringRef networkInterfaceLocalizedDisplayNameString =
            SCNetworkInterfaceGetLocalizedDisplayName(iface);
        QString networkInterfaceLocalizedDisplayNameQString =
            QString::fromCFString(networkInterfaceLocalizedDisplayNameString);
        CFStringRef networkInterfaceBSDNameString =
            SCNetworkInterfaceGetBSDName(iface);
        QString networkInterfaceBSDNameQString =
            QString::fromCFString(networkInterfaceBSDNameString);
        QString networkInterfaceName = networkInterfaceBSDNameQString;
        qDebug() << networkInterfaceName
                 << ": networkInterfaceName: " << __func__;
        if (networkInterfaceName == device_name) {
          en_interface = iface;
        }
      }
    }
  }
  if (!en_interface) {
    errorMessage = tr("找不到用于拨号的网卡 %1").arg(QString(device_name));
    CFRelease(services);
    return false;
  }

  if (!SCPreferencesLock(preferences, 1)) {
    setErrorMessage(errorMessage, "SCPreferencesLock");
    CFRelease(services);
    return false;
  }

  /*SCNetworkInterfaceRef */ ppp_interface =
      SCNetworkInterfaceCreateWithInterface(en_interface,
                                            kSCNetworkInterfaceTypePPP);
  CFRelease(services);
  if (!ppp_interface) {
    setErrorMessage(errorMessage, "SCNetworkInterfaceCreateWithInterface");
    return false;
  }
  ppp_service = SCNetworkServiceCreate(preferences, ppp_interface);
  SCNetworkServiceSetName(ppp_service, name_cf);
  ppp_interface = SCNetworkServiceGetInterface(ppp_service);
  if (!SCNetworkInterfaceSetConfiguration(ppp_interface, dict) ||
      !SCNetworkInterfaceSetConfiguration(ppp_interface, ppp_dict)) {
    setErrorMessage(errorMessage, "SCNetworkInterfaceSetConfiguration");
    return false;
  }
  CFRelease(ppp_dict);

  if (!SCNetworkServiceEstablishDefaultConfiguration(ppp_service)) {
    setErrorMessage(errorMessage,
                    "SCNetworkServiceEstablishDefaultConfiguration");
    return false;
  }
  SCNetworkSetRef set = SCNetworkSetCopyCurrent(preferences);
  if (!set) {
    setErrorMessage(errorMessage, "SCNetworkSetCopyCurrent");
    return false;
  }
  if (!SCNetworkSetAddService(set, ppp_service)) {
    setErrorMessage(errorMessage, "SCNetworkSetAddService");
    return false;
  }
  if (!SCPreferencesCommitChanges(preferences)) {
    setErrorMessage(errorMessage, "SCPreferencesCommitChanges");
    return false;
  }
  if (!SCPreferencesApplyChanges(preferences)) {
    setErrorMessage(errorMessage, "SCPreferencesApplyChanges");
    return false;
  }
  SCPreferencesSynchronize(preferences);
  //	}
  CFStringRef service_id = SCNetworkServiceGetServiceID(ppp_service);

  SCPreferencesUnlock(preferences);

  connection = SCNetworkConnectionCreateWithServiceID(nullptr, service_id,
                                                      nullptr, nullptr);
  if (!connection) {
    setErrorMessage(errorMessage, "SCNetworkConnectionCreateWithServiceID");
    return false;
  }

  SCNetworkConnectionStatus status = SCNetworkConnectionGetStatus(connection);
  if (status != kSCNetworkConnectionDisconnected) {
    errorMessage =
        tr("期待目标连接的状态为 0，但是当前的状态为 %1").arg(status);
    return false;
  }

  if (!SCNetworkConnectionStart(connection, nullptr, 0)) {
    setErrorMessage(errorMessage, "SCNetworkConnectionStart");
    return false;
  }

  do {
    usleep(200 * 1000);
    status = SCNetworkConnectionGetStatus(connection);
  } while (status == kSCNetworkConnectionConnecting);
  return status == kSCNetworkConnectionConnected;
#undef SET_ERROR_MSG
}

void MacOsBaseDsl::hangUp() {
  if (!connection)
    return;
  SCNetworkConnectionStop(connection, 1);
  if (ppp_service) {
    SCNetworkServiceRemove(ppp_service);
    SCNetworkSetRef set = SCNetworkSetCopyCurrent(preferences);
    if (!set)
      return;
    SCNetworkSetRemoveService(set, ppp_service);
    if (!SCPreferencesCommitChanges(preferences))
      return;
    if (!SCPreferencesApplyChanges(preferences))
      return;
    SCPreferencesSynchronize(preferences);
    CFRelease(ppp_service);
    ppp_service = nullptr;
  }
  sleep(1);
}

bool MacOsBaseDsl::isDisconnected() {
  if (!connection)
    return true;
  qDebug() << "connection status = "
           << SCNetworkConnectionGetStatus(connection);

  CFStringRef pppStatusKey = CFSTR("Status");
  auto extendStatus = SCNetworkConnectionCopyExtendedStatus(connection);
  auto pppStatusValue = CFDictionaryGetValue(extendStatus, pppStatusKey);

  qDebug() << __PRETTY_FUNCTION__ << " "
           << "ppp status:" << pppStatusValue;
  return SCNetworkConnectionGetStatus(connection) ==
         kSCNetworkConnectionDisconnected;
}

QStringList MacOsBaseDsl::getAvailableInterfaces() {
  SCPreferencesRef preferences =
      SCPreferencesCreate(nullptr, CFSTR("NSS"), nullptr);
  CFArrayRef services = SCNetworkServiceCopyAll(preferences);
  CFRelease(preferences);
  if (!services)
    return QStringList();
  QStringList networkInterfaceCardList;
  CFIndex service_count = CFArrayGetCount(services);
  for (CFIndex index = 0; index < service_count; ++index) {
    SCNetworkServiceRef service = static_cast<SCNetworkServiceRef>(
        CFArrayGetValueAtIndex(services, index));
    SCNetworkInterfaceRef iface = SCNetworkServiceGetInterface(service);
    CFStringRef name = SCNetworkInterfaceGetLocalizedDisplayName(iface);
    QString qLocalizedDisplayName = QString::fromCFString(name);
    CFStringRef bsdName = SCNetworkInterfaceGetBSDName(iface);
    QString qBsdName = QString::fromCFString(bsdName);
    CFStringRef typeName = SCNetworkInterfaceGetInterfaceType(iface);
    QString qInterfaceTypeName = QString::fromCFString(typeName);

    qDebug() << "Network TypeName: " << qInterfaceTypeName;
    qDebug() << "Network LocalizedDisplayName: " << qLocalizedDisplayName;
    qDebug() << "Network BSDName: " << qBsdName;

    if (SCNetworkInterfaceGetInterfaceType(iface) ==
        kSCNetworkInterfaceTypeEthernet) {
      QString networkInterfaceName = qBsdName;
      if (networkInterfaceName == QString{})
        networkInterfaceName = QString("%1 %2").arg(tr("未知网卡")).arg(index);
      if (!networkInterfaceCardList.contains(networkInterfaceName))
        networkInterfaceCardList.append(networkInterfaceName);
    }
  }
  CFRelease(services);
  return networkInterfaceCardList;
}
