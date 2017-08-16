#ifndef HTTPSJSONPOST_H
#define HTTPSJSONPOST_H
#include <qjson/serializer.h>
#include <QVariant>
#include <qdebug.h>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QtNetwork>
#include "common.h"

#include <QObject>

class HttpsJsonPost : public QObject
{
    Q_OBJECT
public:
    explicit HttpsJsonPost(QObject *parent = 0);
    ~HttpsJsonPost();
    void postJsonData(QVariantMap data);

private:
    void jsonPost();

    //add ssl config
    QSslConfiguration sslConf;

    QNetworkAccessManager nam;
    QNetworkRequest request;
    QNetworkReply *reply;

signals:

public slots:
    void checkError();
};

#endif // HTTPSJSONPOST_H
