#include <basedsl.hpp>
#include <utils.hpp>
BaseDsl::BaseDsl(QObject *parent)
    :QObject{parent},name{NDR_PHONEBOOK_NAME} {

}
