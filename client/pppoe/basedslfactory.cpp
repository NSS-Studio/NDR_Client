#include "basedslfactory.hpp"

#if defined(Q_OS_MAC)
#include <macosbasedsl.hpp>
using PlatformBaseDsl = MacOsBaseDsl;
#elif defined (Q_OS_LINUX)
#include <linuxbasedsl.hpp>
using PlatformBaseDsl = LinuxBaseDsl;
#else
#error "The platform is not supported"
#endif


BaseDsl* BaseDslFactory::getCurrentPlatformBaseDsl() {
    return new PlatformBaseDsl{};
}
