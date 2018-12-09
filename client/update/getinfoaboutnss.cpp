#include "getinfoaboutnss.h"
#include <QNetworkRequest>
#include <QDateTime>
#include <QDomDocument>
#include <QVector>
#include <QDesktopServices>
#include "utils.hpp"
#include <utility>

GetInfoAboutNSS::GetInfoAboutNSS(QString address, QObject *parent)
    : QObject(parent), address{std::move(address)}
{
    qDebug() << "GetInfoAboutNSS()" << endl;
    this->sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    this->sslConf.setProtocol(QSsl::TlsV1_2);
}

GetInfoAboutNSS* GetInfoAboutNSS::getInstance()
{
    static GetInfoAboutNSS* instance = nullptr;
    if(instance == nullptr)
    {
        instance = new GetInfoAboutNSS(NDR_NSS);
    }
    return instance;
}

void GetInfoAboutNSS::checkInfoGet()
{
    qDebug() << "checkInfoGet()" << endl;

    QString url="https://" + address + "/messages/webUp.xml";
    tmp = new QNetworkRequest{QUrl(url)};
    tmp->setSslConfiguration(this->sslConf);

    reply = netGet.get(*tmp);

    connect(reply, &QNetworkReply::finished, this, &GetInfoAboutNSS::checkInfoGetFinish);


    qDebug() << "message url: " << url << endl;
}

//!message format:
//!newmessage/http://172.24.10.13/update/aorigin.xml
//!messageStartTime:Unix time
//!messageEndTime:Unix time
//!
//!
//! <needShow need=1>show</needShow>
//! QString format{"yyyy-MM-dd hh:mm:ss:zzz"};
//! <newmessage http="http://www.neusoft.edu.cn" startTime="2011-09-10 12:07:50:541" endTime="2011-09-10 12:07:50:541">newmessage</newmessage>

void GetInfoAboutNSS::checkInfoGetFinish()
{
    qDebug() << "checkInfoGetFinish()" << endl;
    if(reply->error() != QNetworkReply::NoError)
    {
        //! Because We Get Failed, But We don't need close this client
        qDebug() << "Get MessageInfo Failed: " << reply->errorString() << endl;

        //if failed, retry, need to set retry count
        reply = netGet.get(*tmp);
        return;
    }

    QString stateValue = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();

    qDebug() << "Http state value" << stateValue;
    if(stateValue!="200" && stateValue!="301")
    {
        //! Because We Get Failed, But We don't need close this client
        qDebug() << "Get MessageInfo Http Failed";
        return;
    }
    QByteArray xml;

    xml=reply->readAll();
    qDebug() << "xml string:\n" << xml;

    QDomDocument doc;
    if(doc.setContent(xml,true))
    {
        QDomElement root = doc.documentElement();
        QDomNodeList nodes= root.elementsByTagName("newmessage");

        if(nodes.isEmpty())
        {
            qDebug() << "Get Failed";
            return;
        }

        QDomElement node;
        std::string temp;
        for(int i = 0;i < nodes.size(); ++i)
        {
            node = nodes.at(i).toElement();

            temp = node.attribute("http").toStdString();
            QString http{temp.c_str()};

            temp = node.attribute("startTime").toStdString();
            QString startTime{temp.c_str()};

            temp = node.attribute("endTime").toStdString();
            QString endTime{temp.c_str()};
            messageList.push_back(Message{http, startTime, endTime});
        }

        QDomNodeList needs = root.elementsByTagName("needShow");
        if(needs.isEmpty())
        {
            qDebug() << "Get Failed";
            needShow = false;
            return;
        }

        needShow = needs.at(0).toElement().attribute("need").toInt();

        //needShow just one!!!
        //for(int i = 0;i < needs.size(); ++i)
        //{
        //    node = needs.at(i).toElement();
        //    needShow = node.attribute("need").toInt();
        //}
        readyOpen();
    } else {
        qDebug() << "Get MessageInfo XML format error!";
        return;
    }
}

void GetInfoAboutNSS::readyOpen()
{
    //! 弹网页逻辑
    //! needShow 决定是否显示打开网页logical
    qDebug() << "readyOpen()";
    if (needShow)
        openWeb();
}

void GetInfoAboutNSS::openWeb()
{
    qDebug() << "openWeb()";
    QDateTime currentTime = QDateTime::currentDateTime();
    QVector<QUrl> openUrl;
    QString format{"yyyy-MM-dd hh:mm:ss:zzz"};
    for(auto const& message: messageList)
    {
        QDateTime start = QDateTime::fromString(message.stratTime, format);
        QDateTime end = QDateTime::fromString(message.endTime, format);
        if(start <= currentTime && currentTime <= end)
            openUrl.push_back(QUrl{message.url});
    }

    for (auto url : openUrl)
        QDesktopServices::openUrl(url);

    openUrl.clear();
    messageList.clear();

    qDebug() << "emit endGetInfo";
    emit endGetInfo();
}

GetInfoAboutNSS::~GetInfoAboutNSS()
{
    qDebug() << "delete GetInfoAboutNSS";
    if (tmp)
        delete tmp;
    if (reply)
        reply->deleteLater();
}
