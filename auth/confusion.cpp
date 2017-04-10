#include "confusion.h"

Confusion::Confusion(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < 5; i++) {
        confusion.push_back(new Authenticat());
        ip.push_back(tempIp[i]);
        port.push_back(tempPort[i]);
    }
}

void Confusion::beginVerify() {
    for(int i = 0; i < ip.size(); i++) {
        confusion[i]->beginVerify(ip[i], port[i]);
    }
}
void Confusion::endVerify() {
    for(int i = 0; i < ip.size(); i++) {
        Authenticat::helpEndVerify(confusion[i]);
    }
}
