#include <basedsl.hpp>

BaseDsl::BaseDsl(const QString &name, QObject *parent)
    :QObject{parent},name{name} {

}

BaseDsl::~BaseDsl() {

}
