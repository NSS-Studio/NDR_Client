#ifndef GETINFOABOUTNSS_H
#define GETINFOABOUTNSS_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>


class GetInfoAboutNSS : public QObject
{
    Q_OBJECT
public:
    explicit GetInfoAboutNSS(QString address ,QObject *parent = nullptr);


signals:

public slots:
    void checkInfoGet();
    void checkInfoGetFinish();
private:
    void readyOpen();
    void openWeb();

    QString m_Address;
    QNetworkAccessManager m_NetGet;
    QNetworkReply* m_Reply;
    struct Message
    {
        QString m_Url;
        QString m_StratTime;
        QString m_EndTime;
    };
    QVector<Message> m_Message;
    bool needShow;
};

#endif // GETINFOABOUTNSS_H
