#include "httpgeteventloop.h"

HttpGetEventLoop::HttpGetEventLoop(QObject *parent) :
    QEventLoop(parent)
{
    isError=true;
}

void HttpGetEventLoop::getFinished(bool error)
{
    isError = error;
    this->quit();
}

//bool HttpGetEventLoop::waitForGetting(QHttp &http)
//{
//    connect(&http,SIGNAL(done(bool)),this,SLOT(getFinished(bool)));
//    this->exec();
//    return isError;
//}

bool HttpGetEventLoop::waitForGettingFinished(QNetworkAccessManager *manager)
{
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getFinished(QNetworkReply*)));
    this->exec();
    return isError;
}

void HttpGetEventLoop::getFinished(QNetworkReply *reply)
{
    this->isError = (reply->error()!=QNetworkReply::NoError);
    this->quit();
}
