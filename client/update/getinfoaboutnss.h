#ifndef GETINFOABOUTNSS_H
#define GETINFOABOUTNSS_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QSslConfiguration>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>

class GetInfoAboutNSS : public QObject
{
    Q_OBJECT
public:
    ~GetInfoAboutNSS();
    static GetInfoAboutNSS* getInstance();

signals:
    void endGetInfo();
public slots:
    void checkInfoGet();
    void checkInfoGetFinish();
private:
    explicit GetInfoAboutNSS(QString address ,QObject *parent = nullptr);
    void readyOpen();
    void openWeb();

    QString address;
    QNetworkAccessManager netGet;
    QNetworkRequest *tmp;
    QNetworkReply *reply;
    struct Message
    {
        QString url;
        QString stratTime;
        QString endTime;
    };
    QVector<Message> messageList;
    QSslConfiguration sslConf;

    bool needShow;
};

#endif // GETINFOABOUTNSS_H
