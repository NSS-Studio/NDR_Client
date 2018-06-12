#include "authenticat.h"

Authenticat * Authenticat::instance;


Authenticat::Authenticat() :
    QObject(0)
{
    this->verifyThread=nullptr;
    this->checkFileThread=nullptr;
    this->count = 0;
}

Authenticat * Authenticat::getInstance()
{
    
    if(instance == nullptr) {
        instance=new Authenticat();
    }

    return instance;
}

void Authenticat::beginVerify(QString ip, ushort port)
{
    if(this->verifyThread == nullptr)
    {
        this->verifyThread = new VerifyThread(ip,port);
        this->connect(this->verifyThread, &QThread::started,
                      this, &Authenticat::threadStarted, Qt::QueuedConnection);
        this->connect(this->verifyThread, &QThread::finished,
                      this, &Authenticat::verityThreadFinished, Qt::QueuedConnection);
        this->verifyThread->start();
    }
    if(this->checkFileThread == nullptr)
    {
        this->checkFileThread = new CheckFileThread(ip,port);
        this->connect(this->checkFileThread, &QThread::started,
                      this, &Authenticat::threadStarted, Qt::QueuedConnection);
        this->connect(this->checkFileThread, &QThread::finished,
                      this, &Authenticat::fileThreadFinished, Qt::QueuedConnection);
        this->checkFileThread->start();
    }
}

void Authenticat::helpEndVerify(Authenticat* auth) {
    qDebug() << "Authenticat::helpEndVerify(Authenticat* auth)" << endl;
    if (auth->verifyThread != nullptr)
        auth->verifyThread->kill();
    if (auth->checkFileThread != nullptr)
        auth->checkFileThread->kill();
}

void Authenticat::endVerify() {
    helpEndVerify(getInstance());
    qDebug() << "Authenticat::endVerify()" << endl;
    /*
    delete this->verifyThread;
    delete this->checkFileThread;
    this->verifyThread=nullptr;
    this->checkFileThread=nullptr;
    */
}

void Authenticat::threadStarted()
{
    Log::write("thread started");
    this->count++;
}

void Authenticat::verityThreadFinished()
{
    this->count--;
    if (this->verifyThread != nullptr && this->verifyThread->isFinished())
    {
        delete this->verifyThread;
        this->verifyThread = nullptr;
    } 
    qDebug() << "delete verifyThread end" << endl;
    checkThreadCount();
}

void Authenticat::fileThreadFinished()
{
    this->count--;
    if (this->checkFileThread != nullptr && this->checkFileThread->isFinished())
    {
        delete this->checkFileThread;
        this->checkFileThread = nullptr;
    }
     qDebug() << "delete FileThread end" << endl;
     checkThreadCount();
}

void Authenticat::checkThreadCount()
{
    if(count <= 0)
    {
        count = 0;
        qDebug() << "emit verifyStoped";
        emit verifyStoped();
    }
}



