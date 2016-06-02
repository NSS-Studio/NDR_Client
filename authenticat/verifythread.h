#ifndef VERIFYTHREAD_H
#define VERIFYTHREAD_H

#include <QThread>
#include <QtNetwork>
#include <QAbstractSocket>
#include <string.h>
#include "log.h"
#include <stdint.h>

class VerifyThread : public QThread
{
    Q_OBJECT
public:
    explicit VerifyThread(QString ip,ushort port,QObject *parent=0);
    ~VerifyThread();
    void run();
    void kill();
private:
    
#pragma pack(push,1)
    struct common_header
    {
        int8_t proto;
        int8_t id;
        int16_t length;
        int8_t type;
    };

    struct _challenge{
        struct common_header header;
        int8_t uidlength;
        int16_t unk1;
    };
    struct _challenge_res{
        _challenge challenge;
        unsigned char seed[4];
        unsigned char sip[4];
    };

    struct _heartbeat{
        struct common_header header;
        int8_t uidlength;
        unsigned char mac[6];
        unsigned char sip[4];
        unsigned char unk1[4];
        unsigned char seed[4];
        unsigned char crc[4];
        unsigned char unk2[4];
        unsigned char ips[64];
    };


#pragma pack(pop)
    
    int num1;
    
    QHostAddress host;
    uint16_t port;
    QUdpSocket *client;
    bool stop_now;
    QMutex stop_now_lock;
    int build_challenge(_challenge *pkg);
    void build_heartbeat(unsigned char sip[],unsigned char seed[],_heartbeat *pkg);

public slots:
    
};

#endif // VERIFYTHREAD_H
