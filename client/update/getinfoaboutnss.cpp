#include "getinfoaboutnss.h"
#include <QNetworkRequest>
#include <QDateTime>
#include <QDomDocument>
#include <QVector>
#include <QDesktopServices>
#include "common.h"


GetInfoAboutNSS::GetInfoAboutNSS(QString address, QObject *parent)
    : QObject(parent), m_Address{address}
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

    QString url="https://" + m_Address + "/messages/webUp.xml";
    tmp = new QNetworkRequest{QUrl(url)};
    tmp->setSslConfiguration(this->sslConf);

    m_Reply = m_NetGet.get(*tmp);

    connect(m_Reply, &QNetworkReply::finished, this, &GetInfoAboutNSS::checkInfoGetFinish);


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
    if(m_Reply->error() != QNetworkReply::NoError)
    {
        //! Because We Get Failed, But We don't need close this client
        qDebug() << "Get MessageInfo Failed: " << m_Reply->errorString() << endl;

        //if failed, retry, need to set retry count
        m_Reply = m_NetGet.get(*tmp);
        return;
    }

    QString stateValue = m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();

    qDebug() << "Http state value" << stateValue;
    if(stateValue!="200" && stateValue!="301")
    {
        //! Because We Get Failed, But We don't need close this client
        qDebug() << "Get MessageInfo Http Failed";
        return;
    }
    QByteArray xml;

    xml=m_Reply->readAll();
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
            m_Message.push_back(Message{http, startTime, endTime});
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
    qDebug() << "readyOpen()" << endl;
    if (needShow)
        openWeb();
}

void GetInfoAboutNSS::openWeb()
{
    qDebug() << "openWeb()" << endl;
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

    qDebug() << "emit endGetInfo" << endl;
    emit endGetInfo();
}

GetInfoAboutNSS::~GetInfoAboutNSS()
{
    qDebug() << "delete GetInfoAboutNSS" << endl;
    if (tmp != nullptr)
        delete tmp;
    if (m_Reply != nullptr)
        delete m_Reply;
}
