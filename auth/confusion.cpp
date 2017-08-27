#include "confusion.h"

static QString tempIp[] = {
    "172.24.253.7",
    "172.24.254.7",
    "172.24.251.9",
    "172.25.231.8",
    "172.26.124.67"
};

static quint16 tempPort[] = {
    2345,
    1234,
    2456,
    8888,
    7777
};

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
        qDebug() << "Confusion::endVerify()" << endl;
        Authenticat::helpEndVerify(confusion[i]);
    }
}
