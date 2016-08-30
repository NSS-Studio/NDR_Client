#include "authenticat.h"
Authenticat * Authenticat::instance;

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
    
    if(instance==NULL)
    {
        instance=new Authenticat();
    }
    return instance;
    
}



void Authenticat::beginVerify( QString ip, ushort port )
{
    if(this->verifyThread == NULL)
    {
        this->verifyThread = new VerifyThread(ip,port);
        this->connect(this->verifyThread,SIGNAL(started()),
                      this,SLOT(threadStarted()),Qt::QueuedConnection);
        this->connect(this->verifyThread,SIGNAL(finished()),
                      this,SLOT(threadFinished()),Qt::QueuedConnection);
        this->verifyThread->start( );

        
    }
    if(this->checkFileThread == NULL)
    {
        this->checkFileThread = new CheckFileThread(ip,port);
        this->connect(this->checkFileThread,SIGNAL(started()),
                      this,SLOT(threadStarted()),Qt::QueuedConnection);
        this->connect(this->checkFileThread,SIGNAL(finished()),
                      this,SLOT(threadFinished()),Qt::QueuedConnection);
        this->checkFileThread->start( );
    }

}

void Authenticat::endVerify() {
    
    this->verifyThread->kill();
    this->checkFileThread->kill();
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
    this->count ++;
}

void Authenticat::threadFinished()
{
    qDebug() << "thread stoped";
    //mutex.lock();
    this->count --;
    if(this->verifyThread && this->verifyThread->isFinished())
    {
        delete this->verifyThread;
        this->verifyThread = NULL;
    }
    if(this->checkFileThread && this->checkFileThread->isFinished())
    {
        delete this->checkFileThread;
        this->checkFileThread = NULL;
    }
    if(count <= 0)
    {
        count = 0;
        
        //mutex.unlock();
        qDebug() << "emit verifyStoped";
        emit verifyStoped();
        
    }
    //mutex.unlock();
}




