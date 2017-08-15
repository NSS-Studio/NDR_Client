#include "getinfoaboutnss.h"
#include <QNetworkRequest>
#include <QDateTime>
#include <QDomDocument>
#include <QVector>
#include <QDesktopServices>

GetInfoAboutNSS::GetInfoAboutNSS(QString address, QObject *parent)
    : QObject(parent), m_Address{address}
{

}

void GetInfoAboutNSS::checkInfoGet()
{
    QQString url="http://" + m_Address + "/update/aorigin.xml";
    QNetworkRequest tmp{QUrl(url)};
    tmp.setSslConfiguration(this->sslConf);
    connect(m_Reply,&QNetworkReply::finished, this, &GetInfoAboutNSS::checkInfoGetFinish);
    m_Reply = m_NetGet.get(tmp);
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
    if(m_Reply->error() != QNetworkReply::NoError)
    {
        //! Because We Get Failed, But We don't need close this client
        qDebug() << "Get MessageInfo Failed";
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

        for(int i = 0;i < nodes.size(); ++i)
        {
            QDomElement node = nodes.at(i).toElement();
            QString startTime, endTime;
            QString http = node.attribute("http").toStdString();
            startTime = node.attribute("startTime").toStdString();
            endTime = node.attribute("endTime").toStdString();
            m_Message.push_back(Message{http, startTime, endTime});
        }

        QDomNodeList needs = root.elementsByTagName("needShow");
        if(needs.isEmpty())
        {
            qDebug() << "Get Failed";
            needShow = false;
            return;
        }
        for(int i = 0;i < nodes.size(); ++i)
        {
            QDomElement node = nodes.at(i).toElement();
            needShow = node.attribute("need").toInt();
        }
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
    openWeb();
}

void GetInfoAboutNSS::openWeb()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QVector<QUrl> openUrl;
    QString format{"yyyy-MM-dd hh:mm:ss:zzz"};
    for(int i = 0; i < m_Message.size(); ++i)
    {
        QDateTime start = QDateTime::fromString(m_Message[i].m_StratTime, format);
        QDateTime end = QDateTime::fromString(m_Message[i].m_EndTime, format);
        if(start <= currentTime && currentTime <= end)
            openUrl.push_back(QUrl{m_Message[i].m_Url});
    }
    for(int i = 0; i < openUrl.size(); ++i)
    {
        QDesktopServices::openUrl(openUrl[i]);
    }
}
