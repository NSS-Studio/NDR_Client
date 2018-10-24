#include "httpgeteventloop.h"

HttpGetEventLoop::HttpGetEventLoop(QObject *parent)
    :QEventLoop{parent} {
}

void HttpGetEventLoop::getFinished(bool error) {
    this->isError = error;
    this->quit();
}

bool HttpGetEventLoop::waitForGettingFinished(QNetworkAccessManager *manager)
{
    connect(manager, &QNetworkAccessManager::finished,
            this,
            static_cast<void (HttpGetEventLoop::*)(QNetworkReply *)>
            (&HttpGetEventLoop::getFinished));
    this->exec();
    return isError;
}

void HttpGetEventLoop::getFinished(QNetworkReply *reply)
{
    this->isError = (reply->error() != QNetworkReply::NoError);
    this->quit();
}
