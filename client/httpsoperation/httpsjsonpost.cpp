#include "httpsjsonpost.h"

HttpsJsonPost::HttpsJsonPost(QObject *parent) :
    QObject(parent)
{
    //init
    this->reply = NULL;

    //set ssl config
    this->sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    this->sslConf.setProtocol(QSsl::TlsV1);

}

void HttpsJsonPost::postJsonData(QVariantMap data)
{
    QJson::Serializer serializer;
    bool ok;
    QByteArray json = serializer.serialize(data, &ok);

    if (!ok) {
        return ;
    }
    qDebug() << "success:" << json;
    QString url="https://" + QString(NDR_FEEDBACK_SERVER) + "/upadte.php";
    qDebug() << "url" << url;
    request =QNetworkRequest( QUrl(url));
    request.setSslConfiguration(this->sslConf);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray jsonData = "data=" + json.toBase64();
    reply = nam.post(request, jsonData);
    qDebug() << reply;
    connect(reply,SIGNAL(finished()),this,SLOT(checkError()));
}

void HttpsJsonPost::checkError()
{
    if(reply->error() != QNetworkReply::NoError)
    {
        return ;
    }
    QString stateValue = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();

    qDebug() << "Http state value" << stateValue;
    if(stateValue != "200" && stateValue != "301")
    {
        qDebug() << "远程服务器返回错误状态值" << stateValue;
        return;
    }

    QByteArray retCode;
    retCode = reply->readAll();
    qDebug() << "retCode:\n" << retCode;
    if (retCode == "true")
    {

    }

//    (false)|(true)
}

HttpsJsonPost::~HttpsJsonPost()
{
    if (this->reply != NULL)
    {
        delete this->reply;
    }
}


