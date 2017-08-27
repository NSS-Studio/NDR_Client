#include "authenticat.h"
#include "confusion.h"
Authenticat * Authenticat::instance;
Confusion * Authenticat::confusionInstance;

Authenticat::Authenticat() :
    QObject(0)
{
    this->verifyThread=nullptr;
    this->checkFileThread=nullptr;
    this->count = 0;
    //mutex.unlock();
}

Authenticat * Authenticat::getInstance()
{
    
    if(instance == nullptr) {
        instance=new Authenticat();
    }

    return instance;
}

Confusion* Authenticat::getConfusionInstance() {
    if(confusionInstance == nullptr) {
        confusionInstance = new Confusion();
    }

    return confusionInstance;
}


void Authenticat::beginVerify(QString ip, ushort port)
{
    if(this->verifyThread == nullptr)
    {
        this->verifyThread = new VerifyThread(ip,port);
        this->connect(this->verifyThread, QThread::started,
                      this, Authenticat::threadStarted, Qt::QueuedConnection);
        this->connect(this->verifyThread, QThread::finished,
                      this, Authenticat::verityThreadFinished, Qt::QueuedConnection);
        this->verifyThread->start();
    }
    if(this->checkFileThread == nullptr)
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
    if (verifyThread != nullptr)
        auth->verifyThread->kill();
    if (checkFileThread != nullptr)
        auth->checkFileThread->kill();
}

void Authenticat::endVerify() {
    helpEndVerify(getInstance());
    qDebug() << "Authenticat::endVerify()" << endl;
    getConfusionInstance()->endVerify();
    /*
    delete this->verifyThread;
    delete this->checkFileThread;
    this->verifyThread=nullptr;
    this->checkFileThread=nullptr;
    */
}

void Authenticat::threadStarted()
{
    QMutexLocker lock();
    Log::write("thread started");
    this->count++;
}

void Authenticat::verityThreadFinished()
{
    QMutexLocker lock();
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
    QMutexLocker lock();
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
    QMutexLocker lock();
    if(count <= 0)
    {
        count = 0;
        qDebug() << "emit verifyStoped";
        emit verifyStoped();
    }
}



