#ifndef CONFUSION_H
#define CONFUSION_H

#include <QObject>
#include "authenticat.h"

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
