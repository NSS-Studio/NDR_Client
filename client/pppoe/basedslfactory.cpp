#include "basedslfactory.hpp"

#if defined(Q_OS_MAC)
#include "macosbasedsl.hpp"
using PlatformBaseDsl = MacOsBaseDsl;
#else
#error "The platform is not supported"
#endif


BaseDsl* BaseDslFactory::getCurrentPlatformBaseDsl() {
    return new PlatformBaseDsl{};
}
