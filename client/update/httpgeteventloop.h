#ifndef HTTPGETEVENTLOOP_H
#define HTTPGETEVENTLOOP_H

//! http & https
//! 问题所在 获取xml配置更新文件的时候 需要http
//! 获取下载包的时候 需要使用https


#include <QEventLoop>
//#include <QHttp>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class HttpGetEventLoop : public QEventLoop
{
    Q_OBJECT
public:
    explicit HttpGetEventLoop(QObject *parent = 0);
//    bool waitForGetting(QHttp &http);
    bool waitForGettingFinished(QNetworkAccessManager *manager);
private:
    bool isError;
    QNetworkReply*reply;
signals:
    
public slots:
private slots:
    void getFinished(bool error);
    void getFinished(QNetworkReply *reply);
};

#endif // HTTPGETEVENTLOOP_H
