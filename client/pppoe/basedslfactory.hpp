#ifndef BASEDSLFACTORY_HPP
#define BASEDSLFACTORY_HPP

#include "basedsl.hpp"
namespace BaseDslFactory
{
    QSharedPointer<BaseDsl> getCurrentPlatformBaseDsl();
};

#endif // BASEDSLFACTORY_HPP
