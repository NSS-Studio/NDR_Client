#include "checkfilethread.h"
CheckFileThread::CheckFileThread(QString ip,ushort port,QObject *parent) :
    QThread(parent)
{
    this->host = QHostAddress( ip );
    this->port = port;
    this->stop_now = false;//此处可能需要加互斥锁
    
}
CheckFileThread::~CheckFileThread()
{
    
}
void CheckFileThread::run()
{
    qDebug() << "Check file thread is running.";
#define BUF_SIZE 2048
    char bufRead[BUF_SIZE],bufWrite[BUF_SIZE];
    int readSize,writeSize;
    int ret;
    
    this->client = new QUdpSocket();
    
    bool d=this->client->bind(0);
    
    if(d==false)
    {
        qDebug() << "Port bind failed.";
        goto exit_thread;
    }
    
    for(int i=0;i<5;i++)
    {
        if(stop_now==true)
            goto exit_thread;
        this->sleep(1);
        
    }
    
    num2 = 1;
    writeSize=build_alive("\x00\x00\x00\x00",1,(_alive*)bufWrite);
    forever{
        _alive * pkg = NULL;
        forever{
            if(stop_now)
                goto exit_thread;
            
            Log::write("FSEND:",bufWrite,writeSize);
            ret=client->writeDatagram(bufWrite,writeSize,host,port);
            
            if(ret==-1)
            {
                qDebug() << "client->writeDatagram error";
                this->sleep(2);
                continue;
            }
            if(client->waitForReadyRead(3000))
                break;
            else
                this->sleep(2);
        }
        QHostAddress host_r;
        unsigned short port_r;
        readSize = client->readDatagram(bufRead,BUF_SIZE,&host_r,&port_r);
        
        Log::write("FRECV:",bufRead,readSize);
        pkg = (_alive*)bufRead;
        if(pkg->header.proto==0x07 && pkg->header.type == 0x0B)
        {
            num2 ++ ;
            switch(pkg->type)
            {
            case 0x02:
                writeSize = build_alive(pkg->tail,3,(_alive*)bufWrite);
                break;
            case 0x04:
                writeSize = build_alive(pkg->tail,1,(_alive*)bufWrite);
                for(int i=0;i<20;i++)
                {
                    if(stop_now==true)
                        goto exit_thread;
                    this->sleep(1);
                    
                }
                break;
            case 0x06:
                writeSize = build_alive(pkg->tail,1,(_alive*)bufWrite);
                break;
            default:
                Log::write(QString("CF:Format Error proto:%1, header.type:%2\n").arg(pkg->header.proto).arg(pkg->type));
                Log::write("ERECV:",bufRead,readSize);
                Log::write(QString("Data size = %1\n").arg(readSize));
                goto exit_thread;
                break;
            }
        }else
        {
            //unknow data
            Log::write(QString("CF:Format Error proto:%1, header.type:%2\n").arg(pkg->header.proto).arg(pkg->type));
            Log::write("ERECV:",bufRead,readSize);
            Log::write(QString("Data size = %1\n").arg(readSize));
            goto exit_thread;
        }
    }
exit_thread:
    this->client->close();
    delete this->client;
    qDebug() << "CheckFileThread over";
}

void CheckFileThread::kill()
{
    this->stop_now=true;//此处可能需要加互斥锁
}

int CheckFileThread::build_alive(const char tail[],char type,_alive *pkg)
{
    pkg->header.proto = 0x07;
    pkg->header.id = num2;
    pkg->header.length = sizeof(_alive);
    pkg->header.type = 0x0B;
    pkg->type = type;

    if(num2 == 0)
        memcpy( pkg->flag, "\x0F\x27", sizeof( pkg->flag ) );
    else
        memcpy( pkg->flag, "\xDC\x02", sizeof( pkg->flag ) ) ;
    //Log::write("\xFE\xFE",2);
    memcpy( pkg->unk1,"\x2F\x12\x00\x00\x00\x00\x00\x00",sizeof(pkg->unk1));
    memcpy( pkg->tail, tail, sizeof( pkg->tail ) );
    memset( pkg->unk2, 0, sizeof( pkg->unk2 ) );
    uint32_t tmp;
    if(type == 1)
        tmp = 0;
    else
        tmp = host.toIPv4Address( );
    memcpy( pkg->hostip, &tmp, sizeof( pkg->hostip ) );
    memset( pkg->unk3, 0, sizeof( pkg->unk3 ) );
    return sizeof(_alive);
}

