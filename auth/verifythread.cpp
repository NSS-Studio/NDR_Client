#include "verifythread.h"
#include <QMutexLocker>

VerifyThread::VerifyThread(QString ip,ushort port,QObject *parent) :
    QThread(parent)
{
    this->host = QHostAddress( ip );
    this->port = port;
    QMutexLocker tmpLock(&stop_now_lock);
    this->stop_now = false;//此处可能需要加互斥锁
}

VerifyThread::~VerifyThread()
{

}
void VerifyThread::run()
{
    
    qDebug() << "Verify thread is running. . .";
#define BUF_SIZE 2048
    //QByteArray bufRead,bufWrite;
    char bufRead[BUF_SIZE],bufWrite[BUF_SIZE];
    int readSize, writeSize;
    int ret;
    num1 = 1;
    
    this->client = new QUdpSocket();
    
    //this->client->setSocketOption(QAbstractSocket::LowDelayOption,);
    //this->client->setSocketState(QAbstractSocket::ListeningState);
    
    //this->client->setSocketOption(QAbstractSocket);
    
    
    if(!this->client->bind())
    {
        QAbstractSocket::SocketError err;
        err = client->error();
        qDebug() << "Port bind failed," << err;
        goto exit_thread;
    }
    for(int i=0;i<6;i++)
    {
        {
            QMutexLocker tmpLock(&stop_now_lock);
            if(stop_now==true)
                goto exit_thread;
        }
        this->sleep(1);
    }
    writeSize = build_challenge( ( _challenge* )bufWrite);
    forever{
        common_header * header = NULL;
        forever
        {
            if(stop_now)
                goto exit_thread;
            //ret=client->write(bufWrite);
            //Log::write();
            Log::write("VSEND:",bufWrite,writeSize);
            ret = client->writeDatagram( bufWrite, writeSize, host, port);
            if( ret==-1 )
            {
                qDebug() << "client->writeDatagram error";
                num1 = 1;
                this->sleep(2);
                continue;
            }
            
            if(client->waitForReadyRead(3000))
            {
                break;
            }else
            {
                num1=1;
                writeSize = build_challenge( ( _challenge* )bufWrite);
                this->sleep(2);
            }
        }
        
        readSize = client->readDatagram( bufRead, BUF_SIZE, /*&host*/0, /*&port*/0 );
        Log::write("VRECV:",bufRead,readSize);
        header = ( common_header* )bufRead;
        switch( header->proto )
        {
        case 0x07://验证
            switch( header->type )
            {
            case 0x02:{
                _challenge_res * res = ( _challenge_res * )bufRead;
                num1 = ( num1 + 1 ) % 0xFF;
                
                build_heartbeat( res->sip,res->seed,(_heartbeat*)bufWrite );
                writeSize = 96;
                
            }
                break;
            case 0x04:
                //this->sleep( 15 );
                for(int i=0;i<15;i++)
                {
                    {
                        QMutexLocker tmpLock(&stop_now_lock);
                        if(stop_now==true)
                            goto exit_thread;
                    }
                    this->sleep(1);
                }
                
                num1 = ( num1 + 1 ) % 0xFF;
                writeSize = build_challenge( ( _challenge* )bufWrite);
                break;
            default:;
                Log::write(QString("VF:Format Error proto:%1, header.type:%2\n").arg(header->proto).arg(header->type));
                goto exit_thread;
            }
            break;
        default:;
            Log::write(QString("VF:Format Error proto:%1, header.type:%2\n").arg(header->proto).arg(header->type));
            goto exit_thread;
            //unknow data
        }
    }
exit_thread:;
    this->client->close();
    delete this->client;
    qDebug() << "Verify thread over" ;
}
void VerifyThread::kill()
{
    QMutexLocker tmpLock(&stop_now_lock);
    this->stop_now=true;//此处可能需要加互斥锁
}


int VerifyThread::build_challenge(_challenge *pkg)
{
    pkg->header.proto = 7;
    pkg->header.id = num1;
    pkg->header.length = sizeof( _challenge );
    pkg->header.type = 1;
    pkg->uidlength = 0;
    pkg->unk1 = 0;
    return sizeof(_challenge);
}

void VerifyThread::build_heartbeat(unsigned char sip[],unsigned char seed[],_heartbeat *pkg)
{
    int i,ret;
    pkg->header.proto = 0x07;
    pkg->header.id = num1;
    pkg->header.length = sizeof( _heartbeat );
    pkg->header.type = 3;
    
    memset( pkg->mac, 0, sizeof( pkg->mac ) );
    
    pkg->uidlength = 0;
    memcpy( pkg->sip, sip, sizeof ( pkg->sip ) );
    memcpy( pkg->unk1, "\x00\x63\x00\x14", sizeof( pkg->unk1 ) );
    if( num1 == 1 || num1 == 2 )
        pkg->unk1[1] = 0x62;//第一次
    memcpy(pkg->seed,seed,sizeof(pkg->seed));
    //---
    memcpy( pkg->crc, "\xC7\x2F\x31\x01", sizeof( pkg->crc ) );
    memcpy( pkg->unk2, "\x7E\0\0\0",sizeof(pkg->unk2));
    //memset( pkg->unk2, 0, sizeof( pkg->unk2 ) );
    //pkg->unk2[0] = 0x7E;
    //---
    ret = 0;
    for(i = 0; i < ( (long)&pkg->ips - (long)pkg )/4; i ++)
    {
        ret ^= ((uint32_t*)pkg)[i];
        ret &= (uint32_t)~0;
    }
    
    *(uint32_t*)pkg->crc = ( ret * 19680126 ) & (uint32_t)~0;
    //*(uint32_t*)pkg->crc = ~0;
    memset(pkg->unk2, 0, sizeof(pkg->unk2) );
    memset( pkg->ips, 0, sizeof(pkg->ips) );
}

