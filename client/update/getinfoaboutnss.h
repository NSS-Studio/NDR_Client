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

    QString m_Address;
    QNetworkAccessManager m_NetGet;
    QNetworkRequest *tmp;
    QNetworkReply *m_Reply;
    struct Message
    {
        QString m_Url;
        QString m_StratTime;
        QString m_EndTime;
    };
    QVector<Message> m_Message;
    QSslConfiguration sslConf;

    bool needShow;
};

#endif // GETINFOABOUTNSS_H
