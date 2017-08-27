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
        this->connect(this->verifyThread,SIGNAL(started()),
                      this,SLOT(threadStarted()),Qt::QueuedConnection);
        this->connect(this->verifyThread,SIGNAL(finished()),
                      this,SLOT(threadFinished()),Qt::QueuedConnection);
        this->verifyThread->start();
    }
    if(this->checkFileThread == NULL)
    {
        this->checkFileThread = new CheckFileThread(ip,port);
        this->connect(this->checkFileThread,SIGNAL(started()),
                      this,SLOT(threadStarted()),Qt::QueuedConnection);
        this->connect(this->checkFileThread,SIGNAL(finished()),
                      this,SLOT(threadFinished()),Qt::QueuedConnection);
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

void Authenticat::threadFinished()
{
    qDebug() << "thread stoped";
    //mutex.lock();
    this->count--;
    qDebug() << "delete verifyThread begin" << endl;
    if(this->verifyThread && this->verifyThread->isFinished())
    {
        this->verifyThread->deleteLater();
        //delete this->verifyThread;
        //this->verifyThread = NULL;
    }
    if(this->checkFileThread && this->checkFileThread->isFinished())
    {
        this->checkFileThread->deleteLater();
        //delete this->checkFileThread;
        //this->checkFileThread = NULL;
    }
    qDebug() << "delete verifyThread end" << endl;
    if(count <= 0)
    {
        count = 0;
        
        //mutex.unlock();
        qDebug() << "emit verifyStoped";
        emit verifyStoped();
        
    }
    //mutex.unlock();
}





