#include "basedslfactory.hpp"
#include <QSharedPointer>
#if defined(Q_OS_MAC)
#include <macosbasedsl.hpp>
using PlatformBaseDsl = MacOsBaseDsl;
#elif defined (Q_OS_LINUX)
#include <linuxbasedsl.hpp>
using PlatformBaseDsl = LinuxBaseDsl;
#elif defined (Q_OS_WIN32)
#include <windowsbasedsl.hpp>
using PlatformBaseDsl = WindowsBaseDsl;
#else
#error "The platform is not supported"
#endif

namespace BaseDslFactory {
    static QSharedPointer<BaseDsl> platformBaseDsl{new PlatformBaseDsl{}};
    QSharedPointer<BaseDsl> getCurrentPlatformBaseDsl() {
        return platformBaseDsl;
    }
}


