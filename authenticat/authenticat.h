#ifndef AUTHENTICAT_H
#define AUTHENTICAT_H
#include <QtCore>
#include <QThread>
#include <QDebug>
//#include <QHostAddress>
#include <QtNetwork>
#include <stdlib.h>
#include "checkfilethread.h"
#include "verifythread.h"
class  Authenticat : public QObject{
    Q_OBJECT
public:
    static Authenticat *getInstance();//构造单例, 进程只允许一个心跳实例
    /**
     * @brief beginVerify 开启心跳认账
     * @param ip 认证服务器地址
     * @param port 端口
     */
    void beginVerify(QString ip,ushort port);//开始验证

    /**
     * @brief endVerify 结束心跳认证
     */
    void endVerify();
private:

    Authenticat();
    
    VerifyThread * verifyThread;
    CheckFileThread * checkFileThread;
    static Authenticat * instance;//单例模式
    int count;
    QMutex mutex;
private slots:
    void threadStarted();
    void threadFinished();
protected:
signals:
    void verifyStoped();//认证结束信号
};

#endif // AUTHENTICAT_H
