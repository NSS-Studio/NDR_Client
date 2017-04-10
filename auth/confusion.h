#ifndef CONFUSION_H
#define CONFUSION_H

#include <QObject>
#include "authenticat.h"

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

class Confusion : public QObject
{
    Q_OBJECT
public:
    explicit Confusion(QObject *parent = 0);
    void beginVerify();
    void endVerify();

private:
    QVector<Authenticat*> confusion;
    QVector<QString> ip;
    QVector<quint16> port;
};
#endif // CONFUSION_H
