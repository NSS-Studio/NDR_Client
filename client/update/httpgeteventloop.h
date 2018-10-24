#ifndef HTTPGETEVENTLOOP_H
#define HTTPGETEVENTLOOP_H

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class HttpGetEventLoop : public QEventLoop
{
    Q_OBJECT
public:
    explicit HttpGetEventLoop(QObject *parent = nullptr);
    bool waitForGettingFinished(QNetworkAccessManager *manager);
private:
    bool isError = true;
    QNetworkReply* reply;
private slots:
    void getFinished(bool error);
    void getFinished(QNetworkReply *reply);
};

#endif // HTTPGETEVENTLOOP_H
