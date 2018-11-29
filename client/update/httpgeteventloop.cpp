#include "httpgeteventloop.h"

HttpGetEventLoop::HttpGetEventLoop(QObject *parent)
    :QEventLoop{parent} {
}

void HttpGetEventLoop::getFinished(bool error) {
    isError = error;
    quit();
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
    isError = (reply->error() != QNetworkReply::NoError);
    quit();
}
