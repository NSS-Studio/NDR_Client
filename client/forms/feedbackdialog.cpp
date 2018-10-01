#include "feedbackdialog.h"
#include "ui_feedbackdialog.h"
#include <QDesktopWidget>
#include <utils.hpp>
FeedbackDialog::FeedbackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeedbackDialog)
{
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    ui->sldScore->setSliderPosition(3);
    ui->chkToScore->setChecked(false);
    ui->sldScore->setEnabled(false);
    ui->lblWord->setText(tr("点评分前面的勾勾进行打分"));

    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_2);
}

FeedbackDialog::~FeedbackDialog()
{
    delete ui;
}



void FeedbackDialog::on_sldScore_sliderMoved(int position)
{
    QString word;
    QColor color;
    switch(position)
    {
    case 1:
        word = tr("反人类设计，BUG一堆，开发者们都去食屎吧！");
        color=Qt::red;
        break;
    case 2:
        word = tr("相比原版客户端差远了，快点更新！");
        color=QColor::fromRgb(255,150,0);
        break;
    case 3:
        word = tr("能凑合用，起码有网比没网强 = = ");
        color=QColor::fromRgb(150,150,0);
        break;
    case 4:word = tr("用起来还算不错，但在某些功能上还需改善");
        color=QColor::fromRgb(100,200,20);
        break;
    case 5:word = tr("超级好用啊，给开发者点赞！");
        color=QColor::fromRgb(50,240,0);
        break;
    default:word = tr("你是怎么做到的？");
        color=Qt::cyan;
        break;
    }
    word =tr("%1分 ").arg(position) + word;
    QPalette palette;
    palette.setColor(QPalette::WindowText,color);
    ui->lblWord->setText(word);
    ui->lblWord->setPalette(palette);
}

void FeedbackDialog::on_chkToScore_clicked(bool checked)
{
    if(checked)
    {
        ui->sldScore->setEnabled(true);
        this->on_sldScore_sliderMoved(ui->sldScore->value());
    }else
    {
        ui->sldScore->setEnabled(false);
        this->ui->lblWord->setText("");
    }
}

void FeedbackDialog::on_btnSubmit_clicked()
{
    QString caption=ui->cmbFeedback->currentText();
    if(caption.length()<5)
    {
        QMessageBox::information(this,tr("提示"),tr("标题必须保证5个字符以上"));
        return;
    }
    QString uname = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).section("/", -1, -1);
    QString mach = QHostInfo::localHostName();
   
    QString detail=this->ui->textDetail->toPlainText();
    QString star = QString::number(this->ui->chkToScore->isChecked()?ui->sldScore->value():0);
    QString contact = ui->txtContactValue->text();
    if(contact.isEmpty())
        contact = "";
    else
        contact = ui->cmbContact->itemText(ui->cmbContact->currentIndex()) + " " + contact;
    qDebug() << "contact" << contact;
    QString os;
#ifdef Q_OS_WIN32
    os="0";
#elif defined Q_OS_LINUX
    os="1";
#elif defined Q_OS_MAC
    os="3";
#else
    os="-1"
#endif
    
    //http://172.24.10.118/ndr/report.php?action=user_report
   
    QString urlStr=QString("https://%0/feedback/report.php").arg(NDR_FEEDBACK_SERVER);
    qDebug() << "url_str" << urlStr;
    QUrl urlFeedback(urlStr);
    QUrlQuery query;
    query.addQueryItem("action","user_report");
    urlFeedback.setQuery(query);
//    urlFeedback.addEncodedQueryItem("action","user_report");
    request.setSslConfiguration(sslConf);
    request.setUrl(urlFeedback);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    
    qDebug() << "url" << urlFeedback.toString();
    QByteArray postData = (QString(
            "account="+account.toUtf8().toPercentEncoding()+
            "&sd="+caption.toUtf8().toPercentEncoding()+
            "&fd="+detail.toUtf8().toPercentEncoding()+
            "&uname="+uname.toUtf8().toPercentEncoding()+
            "&mach="+mach.toUtf8().toPercentEncoding()+
            "&os="+os.toUtf8().toPercentEncoding()+
            "&ct="+contact.toUtf8().toPercentEncoding()+
            "&star="+star.toUtf8().toPercentEncoding()+
            "&combo="+combo.toUtf8().toPercentEncoding()).toUtf8());

    qDebug() << "post_data" << postData;
    reply = nam.post(request,postData);
	connect(reply,SIGNAL(finished()),this,SLOT(postFinished()));
    
    //this->ui->btnSubmit->setEnabled(false);
    

}

#define ERROR_UNKNOW_FORMAT tr("反馈发送失败，无法识别的协议格式。")
void FeedbackDialog::postFinished()
{
    QString errorMessage;
    qDebug() << "postFinished() enter";
    QString httpStatusCode
            =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    QByteArray xml;
    QDomDocument doc;
    if(reply->error() != QNetworkReply::NoError)
    {
        errorMessage = tr("连接服务器失败");
        goto failed ;
    }
    if(httpStatusCode!="200")
    {
        errorMessage = tr("请求服务器失败, 返回状态代码%1").arg(httpStatusCode);
        goto failed;
    }
	
    
	xml=reply->readAll();
    qDebug() << "xml" << xml;
    
    
	if(doc.setContent(xml,true))
	{
        bool ok;
        int code;
        QString message;
		QDomElement root = doc.documentElement();
		QDomNodeList nodes = root.elementsByTagName("code");
		if(!nodes.isEmpty()){
			code = nodes.at(0).toElement().text().toInt(&ok);
            if(!ok)
            {
                errorMessage=ERROR_UNKNOW_FORMAT;
                goto failed;
            }
            
		}else
        {
            errorMessage = ERROR_UNKNOW_FORMAT;
			goto failed;
        }
        
        nodes = root.elementsByTagName("message");
        if(!nodes.isEmpty()){
			message = nodes.at(0).toElement().text();
		}else
        {
            errorMessage = ERROR_UNKNOW_FORMAT;
			goto failed;
        }
        if(0!=code)
        {
            if(message == "ERR_INTERNAL_ERROR")
            {
                errorMessage = tr("反馈服务器内部错误");
            }
            else if(message == "FUCK_YOU_MOTHER")
            {
                errorMessage = tr("您提交的过于频繁");
            }
            else if(message == "NO_ENOUGH_PARAM")
            {
                errorMessage = tr("参数不足");
            }
            else
            {
                errorMessage = tr("无法解析的错误");
            }
            goto failed;
        }

    }
    


    this->hide();
    this->ui->cmbFeedback->setCurrentIndex(0);
    this->ui->chkToScore->setCheckable(false);
    this->ui->lblWord->setText("");
    this->ui->textDetail->clear();
    this->ui->txtContactValue->clear();
    QMessageBox::information(this,tr("反馈成功"),tr("感谢您反馈，我们会考虑并尽力采纳您的意见"));
    return;
    failed:
    QMessageBox::information(this,tr("反馈失败"),errorMessage);
    qDebug() << "postFinished() exit";
	//QDomDocument doc;
}

void FeedbackDialog::setLoginData(QString account,QString combo)
{
    this->account = account;
    this->combo = combo;
}

void FeedbackDialog::show()
{
	QDesktopWidget * desktop = QApplication::desktop();
	move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
	QDialog::show();
}

