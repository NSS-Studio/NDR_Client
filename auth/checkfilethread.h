#ifndef CHECKFILETHREAD_H
#define CHECKFILETHREAD_H

#include <QThread>
#include <QtNetwork>
#include <string.h>
#include <stdio.h>
//#include "log.h"
#include <stdint.h>

class CheckFileThread : public QThread
{
    Q_OBJECT
public:
    explicit CheckFileThread(QString ip,ushort port,QObject *parent=0);
    ~CheckFileThread();
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
    struct _alive{
        struct common_header header;
        int8_t type;
        char flag[2];
        char unk1[8];
        char tail[4];
        char unk2[8];
        char hostip[4];//if type==1 hostip=00H*4
        char unk3[8];
        
    };
    struct _alive_res{
        struct common_header header;
        char fuckout[11];
        char tail[4];
    };
#pragma pack(pop)
    char num2;
    bool stop_now;
    uint16_t port;
    QHostAddress host;
    QUdpSocket *client;
    int build_alive(const char tail[],char type,_alive *pkg);
signals:
    
public slots:
    
};

#endif // CHECKFILETHREAD_H
