#include "authenticat.h"
#include "confusion.h"
Authenticat * Authenticat::instance;
Confusion * Authenticat::confusionInstance;

Authenticat::Authenticat() :
    QObject(0)
{
    this->verifyThread=NULL;
    this->checkFileThread=NULL;
    this->count = 0;
    //mutex.unlock();
}

Authenticat * Authenticat::getInstance()
{
    
    if(instance == NULL) {
        instance=new Authenticat();
    }

    return instance;
}

Confusion* Authenticat::getConfusionInstance() {
    if(confusionInstance == NULL) {
        confusionInstance = new Confusion();
    }

    return confusionInstance;
}


void Authenticat::beginVerify(QString ip, ushort port)
{
    if(this->verifyThread == NULL)
    {
        this->verifyThread = new VerifyThread(ip,port);
        this->connect(this->verifyThread, QThread::started,
                      this, Authenticat::threadStarted, Qt::QueuedConnection);
        this->connect(this->verifyThread, QThread::finished,
                      this, Authenticat::verityThreadFinished, Qt::QueuedConnection);
        this->verifyThread->start();
    }
    if(this->checkFileThread == NULL)
    {
        this->checkFileThread = new CheckFileThread(ip,port);
        this->connect(this->checkFileThread, QThread::started,
                      this, Authenticat::threadStarted, Qt::QueuedConnection);
        this->connect(this->checkFileThread, QThread::finished,
                      this, Authenticat::fileThreadFinished, Qt::QueuedConnection);
        this->checkFileThread->start();
    }
}

void Authenticat::helpEndVerify(Authenticat* auth) {
    qDebug() << "Authenticat::helpEndVerify(Authenticat* auth)" << endl;
    auth->verifyThread->kill();
    auth->checkFileThread->kill();
}

void Authenticat::endVerify() {
    helpEndVerify(getInstance());
    qDebug() << "Authenticat::endVerify()" << endl;
    getConfusionInstance()->endVerify();
    /*
    delete this->verifyThread;
    delete this->checkFileThread;
    this->verifyThread=NULL;
    this->checkFileThread=NULL;
    */
}

void Authenticat::threadStarted()
{
    Log::write("thread started");
    this->count++;
}

void Authenticat::verityThreadFinished()
{
    qDebug() << "thread stoped";

    this->count--;
    qDebug() << "delete verifyThread begin" << endl;
    if (this->verifyThread->isFinished())
    {
        delete this->verifyThread;
        this->verifyThread = NULL;
    } 

    qDebug() << "delete verifyThread end" << endl;
    checkThreadCount();
}

void Authenticat::fileThreadFinished()
{
    this->count--;
    if (this->checkFileThread->isFinished())
    {
        delete this->checkFileThread;
        this->checkFileThread = NULL;
    }
     qDebug() << "delete FileThread end" << endl;
     checkThreadCount();
}

void Authenticat::checkThreadCount()
{
    QMutexLocker lock();
    if(count <= 0)
    {
        count = 0;
        qDebug() << "emit verifyStoped";
        emit verifyStoped();
    }
}



