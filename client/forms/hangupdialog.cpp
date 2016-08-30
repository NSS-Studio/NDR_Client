#include "hangupdialog.h"
#include "ui_hangupdialog.h"

HangupDialog::HangupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HangupDialog)
{
    setWindowFlags(Qt::WindowTitleHint 
                   | Qt::CustomizeWindowHint);
    ui->setupUi(this);
#ifdef Q_OS_LINUX
	//ui->btnDelay->setR
	//ui->btnDelay->setFixedWidth(80);
	//ui->btnDelay->setText(tr("稍后提醒") + "(&D)");
	//ui->btnAccept->setText(tr("打开软件包所在目录"));
	//ui->label->setText(tr("NDR 已经将新版本软件包下载到了临时目录，请打开目录并手动安装。如果您不想立即更新，请选择下次提示的时间并单击“%1”").arg(ui->btnDelay->text()));
#endif
}

HangupDialog::~HangupDialog()
{
    delete ui;
}

void HangupDialog::closeEvent(QCloseEvent *event)
{
    if(!close_now)
        event->ignore();
    
}

bool HangupDialog::event(QEvent *e)
{
    if(e->type() == QEvent::KeyPress && ((QKeyEvent*)e)->key() == Qt::Key_Escape)
    {
        return true;
    }
    return QWidget::event(e);
}

void HangupDialog::waitForUserAccept()
{
    //close_now = false;
    restTime = 60*3;
    timerId = this->startTimer(1000);
    this->show();
    loop.exec();
    this->hide();
    this->killTimer(timerId);
    
}

void HangupDialog::timerEvent(QTimerEvent *t)
{
    qDebug() << "Rest time" << restTime;
    restTime --;
    if(this->restTime <= 0)
    {
        //this->close_now=true;
        loop.quit();
    }
    if(this->restTime <= 60*3)
    {
		this->ui->lcdLefttime->display(restTime);
        if(!this->isVisible())
        {
            this->show();
            this->activateWindow();
        }

    }
}

void HangupDialog::on_btnDelay_clicked()
{
    int delayTime;
    qDebug() << "on_btnDelay_clicked";
    switch(ui->cmbDelay->currentIndex())
    {
    case 0:delayTime=60*5;break;
    case 1:delayTime=60*20;break;
    case 2:delayTime=60*60;break;
    default:delayTime=60*1;break;
    }
    delayTime += 60*3;
    this->restTime = delayTime;
    this->hide();
}

void HangupDialog::on_btnAccept_clicked()
{
    loop.quit();
}

void HangupDialog::setMessage(const QString &message) {
	ui->label->setText(QString("<p style=\"font-size:8pt; color:#000000;\">%1</p>").arg(message));
}

void HangupDialog::setAcceptButtonText(const QString &text) {
	ui->btnAccept->setText(text);
}

void HangupDialog::setDelayButtonText(const QString &text) {
	ui->btnDelay->setText(text);
}
