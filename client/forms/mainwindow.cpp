#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "singleapplication.h"
#include <QMessageBox>

/*
#ifdef Q_OS_MAC
#include <QtGui/QMacStyle>
#endif
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //this->setWindowFlags(Qt::Dialog);
    this->setWindowFlags(this->windowFlags()|Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);

    //settings = new SettingsSet(appHome + "/config.ini");

    logoffShortcut = new QxtGlobalShortcut(this);
    connect(logoffShortcut,SIGNAL(activated()),this,SLOT(logoffShortcutActivated()));
    if(!settings->hotkey.isEmpty() && !logoffShortcut->setShortcut(QKeySequence(settings->hotkey)))
    {
        QMessageBox::critical(this,tr("错误"),tr("注销快捷键注册失败,快捷键无效或者可能已经被其他应用程序占用。"));
        
    }else
    {
        logoffShortcut->setDisabled();
    }

    profile = new LocalStorage(appHome + "/config.db");//如果数据库结构变化，修改文件名抛弃数据

    this->ui->menuTrayLogin->menuAction()->setVisible(false);
    this->ui->menuTrayWorking->menuAction()->setVisible(false);
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        trayIcon=NULL;
        qDebug("System tray is not available, tray icon disabled");
        return;
    }
    trayIcon = new QSystemTrayIcon();
    this->trayIcon->show();
    this->connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                  this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    
    this->pppoe = new PPPoE();
    connect(this->pppoe,SIGNAL(dialFinished(bool)),
            this,SLOT(dialFinished(bool)),Qt::QueuedConnection);
    connect(this->pppoe,SIGNAL(redialFinished(bool)),
            this,SLOT(redialFinished(bool)),Qt::QueuedConnection);
    connect(this->pppoe,SIGNAL(hangedUp(bool)),
            this,SLOT(hangedUp(bool)),Qt::QueuedConnection);
    
    this->loginDialog = new LoginDialog(profile);
    
#ifndef Q_OS_WIN
    QStringList interfaces = PPPoE::getAvailableInterfaces();
    if(interfaces.count() == 0) {
	    QMessageBox::critical(this, tr("NDR"), tr("No interface available"));
	    QApplication::instance()->exit(1);
		loginDialog->close();
		delete loginDialog;
		close();
		((SingleApplication *)QApplication::instance())->releaseSharedMemory();
		exit(1);
		return;
    }
	this->loginDialog->set_interface_list(interfaces);
#endif
    connect(this->loginDialog,SIGNAL(myaccepted()),
            this,SLOT(tryLogin()));
    connect(this->loginDialog,SIGNAL(finished(int)),this,SLOT(loginWindowClosed()));

    this->noticeDialog = new NoticeDialog();
    
    connect(Authenticat::getInstance(),SIGNAL(verifyStoped()),
                  this,SLOT(verifyStoped()),Qt::QueuedConnection);
    
    
    connect(this,SIGNAL(minimumWindow()),this,SLOT(hide()),Qt::QueuedConnection);//绑定最小化到隐藏
    
	isMainWindowMinimized=false;

    //更新模块初始化
    updateServer = new UpdateService(NDR_UPDATE_SERVER,NDR_UPDATE_SERVER_2_BACK,tempDir);
    connect(updateServer,SIGNAL(checkFinished(bool,int,int,QString)),
            this,SLOT(checkFinished(bool,int,int,QString)));
    connect(updateServer,SIGNAL(downloadFinished(bool,QString)),
            this,SLOT(downloadFinished(bool,QString)));

    //网卡信息类初始化
    interfaceInfo = new InterfaceInfo();              //测试性调用接口，使用完成后删除
    interfaceInfo->getInterfaceInfo("以太网","ppp0");
    qDebug() << "flag from mainwindows:";
    QString *aaa = interfaceInfo->getHwAddress();
    if (aaa != NULL)
        qDebug() << "interface information getHwAddress:" << *aaa;
    else
        qDebug() << "interface hardware address not found!";
    QString *bbb = interfaceInfo->getIpAddress();
    if (bbb != NULL)
        qDebug() << "interface information getIpAddress:" << *bbb;
    else
        qDebug() << "interface ip address not found!";
    this->ui->lblAllTime->setText("NULL");
    this->ui->lblFlow->setText("NULL");

    aboutDialog = NULL;
    settingsDialog = NULL;
    feedbackDialog = NULL;
    app_exiting = false;

	onStartLogining();
    
    /***/
    //updateServer->checkUpdate();
}

MainWindow::~MainWindow()
{
//    if (usrInfo)
//        delete usrInfo;

    if(updateServer)
        delete updateServer;

    delete loginDialog;
    if(aboutDialog)
        delete aboutDialog;
    if(settingsDialog)
        delete settingsDialog;

    if(feedbackDialog)
        delete feedbackDialog;
    delete pppoe;
    trayIcon->hide();
    delete trayIcon;
    delete ui;

    delete profile;
    delete logoffShortcut;
    //delete settings;
}

void MainWindow::tryLogin()
{
    QString username, password, device_name, errorMessage;
    QString realUsername;
    QString postfix;
	onStopLogining();
    loginDialog->getFormData(username,password,postfix,device_name);
    this->ui->lblAccount->setText(username);

    // h
    QString model_caption;
    if(__getDrModelCaption(postfix,model_caption))
        this->ui->lblType->setText(model_caption);
    else
        this->ui->lblType->setText(tr("未知"));
    realUsername="\r\n" + username + postfix;//+ "@student";

    noticeDialog->showMessage(tr("正在拨号. . ."));
    pppoe->dialRAS(NDR_PHONEBOOK_NAME, realUsername, password, device_name);
    //this->show();
    //Authenticat::getInstance()->beginVerify(DRCOM_SERVER_IP,DRCOM_SERVER_PORT);
    
}

QString MainWindow::time_humanable(int sec)
{
    int day,hour,minute,second;
    int t;
    t = 24*3600;
    day = sec / t;
    sec -= t*day;
    t = 3600;
    hour = sec / t;
    sec -= t*hour;
    t = 60;
    minute = sec / t;
    sec -= t*minute;
    second = sec;
    //return QString(year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second);
    return tr("%1 天 %2:%3:%4")
            .arg(day,-3,10,QChar(' '))
            .arg(hour,2,10,QChar('0'))
            .arg(minute,2,10,QChar('0'))
            .arg(second,2,10,QChar('0'));
    
}

void MainWindow::dialFinished(bool ok)
{
    qDebug() << QString("dialFinished(%1) enter").arg(ok);
    if(ok)
    {
        if(profile->open())
        {
		QString username, password, device_name;
		QString manner;
		bool autoLogin,savePassword;
		loginDialog->getFormData(username,password,manner,device_name,&autoLogin,&savePassword);
		profile->setLoginInfo(username,savePassword?password:"",manner);
		profile->setDeviceName(device_name);	// Deprecated
		QSettings conn_cfg(appHome + "/connection.cfg", QSettings::IniFormat);
		conn_cfg.setValue("Interface/Etherface", device_name);
		this->username = username;
		int left,top,width,height;
		int desktop_width = QApplication::desktop()->width();
		int desktop_height = QApplication::desktop()->height();
		if(profile->getMainWindowRect(username,left,top,width,height))
		{

                if(left + this->width() > desktop_width - 100 || left + this->width() < 100
                        || top + this->height() > desktop_height - 100 || top + this->height() < 100)
                {
                    left = desktop_width - this->width() - 100;
                    top = (desktop_height - this->height())/2;
                }
                ///QMessageBox::information(this,"",QString::number(width)+ " " + QString::number(height));
                this->move(left,top);
                //this->resize(width,height);
            }else
            {
                left = desktop_width - this->width() - 100;
                top = (desktop_height - this->height())/2;
                this->move(left,top);
            }

            if(!profile->getUserOnlineTime(username,allTime))
            {
                this->allTime = 0;
            }
            
            this->ui->lblAllTime->setText(time_humanable(this->allTime));
            
            if(autoLogin)
                profile->setAutoLoginUser(username);
            else
            {
                QString autoLoginUserName;
                if(profile->getAutoLoginUser(autoLoginUserName) 
                        && autoLoginUserName ==username)
                    profile->setAutoLoginUser("");
            }
            profile->close();

        }

        noticeDialog->showMessage(tr("拨号成功，开启认证"));
        this->ui->lblAddress->setText(pppoe->getIpAddress());

        this->connTime = 0;
        this->ui->lblTime->setText(time_humanable(connTime));
        
        
        onStartWorking();
        QEventLoop eventloop;
        QTimer::singleShot(800, &eventloop, SLOT(quit()));
        eventloop.exec();
        if(settings->autoMinimize)
        {
            this->isMainWindowMinimized=true;
            this->hide();
            this->trayIcon->showMessage(tr("NDR 校园网络认证"),tr("主面板已最小化到这里，您可以进入设置关闭自动最小化功能。"),QSystemTrayIcon::Information,4000);
        }
        noticeDialog->hide();
        Authenticat::getInstance()->beginVerify(DRCOM_SERVER_IP,DRCOM_SERVER_PORT);//必须在beginworkingui前
        if(ENABLE_UPDATE)
            updateServer->checkUpdate();
    }else
    {
        noticeDialog->hide();
        onStartLogining();
#ifdef Q_OS_MAC
        if(!pppoe->lastError().isEmpty())
#endif
        QMessageBox::information(loginDialog,tr("提示"),tr("拨号失败") + "\n" + pppoe->lastError());
    }
    qDebug() <<"dialFinished() exit";
}


void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
#ifndef Q_OS_MAC
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
/*
        if(state==Working)
        {
            this->show();
            this->activateWindow();
		isMainWindowMinimized=false;
        }
        else if(state==Logining)
        {
            loginDialog->show();
            loginDialog->activateWindow();
        }*/
		on_actionShowWindow_triggered();
		break;
    default:
        break;
    }
#endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (settings->quitWhileCloseWindow)
    {
        this->ui->actionLogoff->trigger();
        app_exiting = true;
    }else
    {
        this->hide();
    }
    event->ignore();
}

void MainWindow::changeEvent(QEvent* event)
{
    if(event->type()==QEvent::WindowStateChange)
    {
        if(windowState() & Qt::WindowMinimized)
        {
            emit minimumWindow();
		isMainWindowMinimized=true;
            //this->showNormal();
            //event->ignore();
        }

        QMainWindow::changeEvent(event);
        //this->QMainWindow::changeEvent(event);
        
    }
}

void MainWindow::on_actionQuit_triggered()
{
    if(state==Working) {
        app_exiting = true;
        this->ui->actionLogoff->trigger();
        return;
    }else{
        qApp->exit(0);
    }
}


void MainWindow::on_actionShowWindow_triggered()
{
	//奇怪
	if(state == Working) {
		this->show();
		this->activateWindow();
		isMainWindowMinimized = false;
	} else if(state == Logining) {
		//loginDialog->showNormal();
		loginDialog->show();
		loginDialog->activateWindow();
	}
}


void MainWindow::on_actionShowLoginDialog_triggered()
{
	/*
	if(state == Logining) {
		loginDialog->showNormal();
		loginDialog->activateWindow();
	}*/
}

void MainWindow::on_actionLogoff_triggered()
{
    qDebug("slot: on_actionLogoff_triggered()");
    onStopWorking();
    noticeDialog->showMessage(tr("正在尝试注销"));
    Authenticat::getInstance()->endVerify();
    this->pppoe->hangUp(); 

}

void MainWindow::on_actionAbout_triggered()
{
	if(aboutDialog==NULL)
		aboutDialog = new AboutDialog();
	if(aboutDialog->isVisible())
		aboutDialog->activateWindow();
	else
		aboutDialog->show();
}

void MainWindow::verifyStoped()
{
    qDebug() << "verifyStoped() enter";
    {
        //trayIcon->show();
        noticeDialog->hide();//重播失败，正在关闭验证
        if(!this->app_exiting) {

			//onStartLogining();
			//beginLoginUI();

		}
    }
    qDebug() << "verifyStoped() exit";
    
}



static bool _get_account_state(int *time,int *flow)
{
    Q_ASSERT(time);
    Q_ASSERT(flow);
    QNetworkAccessManager nam;
    QNetworkReply *reply;
    QEventLoop eventloop;

    reply = nam.get(QNetworkRequest( QUrl("http://172.24.253.35/")));
    QObject::connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QTimer::singleShot(10*1000,&eventloop,SLOT(quit()));
    eventloop.exec();
    //this->ui->textBrowser->setText(reply->readAll());
    if(!reply->isFinished()) {
        reply->abort();
        qDebug() << "_get_account_state" << "http request timeout";
        return false;
    }
    QString stateValue = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();

    if(stateValue != "200") {
        qDebug() << "_get_account_state" << "stauts code not 200.";
        return false;

    }
    QString html = reply->readAll();
    QRegExp regexp("time='(\\d+)[ ]*';flow='(\\d+)[ ]*';");
    int pos=html.indexOf(regexp);
    if(pos >= 0) {
        qDebug() << "matched string ->" << regexp.capturedTexts();
        *time = regexp.cap(1).toInt();
        *flow = regexp.cap(2).toInt();
        qDebug() << "time:" << *time;
        qDebug() << "time:" << *flow;
        return true;
    } else {
        qDebug() << "_get_account_state" << "unmatched.";
        return false;
    }
}
void MainWindow::timerEvent( QTimerEvent * )
{
    /*
    static int i=0;
    if(++i>=10)
    {
        i=0;
        if(profile->open())
        {
            profile->setUserOnlineTime(this->username,this->allTime);
            profile->close();
        }
    } 
    connTime +=1;
    allTime +=1;
    this->ui->lblTime->setText(parseSec(connTime));
    this->ui->lblAllTime->setText(parseSec(allTime));
    */
    static int timepassed = 0;

    int time,flow;
    if(_get_account_state(&time,&flow)) {
        this->ui->lblTime->setText(time_humanable(timepassed*60));
        this->ui->lblAllTime->setText(time_humanable(time*60));
        this->ui->lblFlow->setText(QString("%1 KB")
                                   .arg(flow,-3,10,QChar(' ')));
    }

    timepassed += 1;

    
}

void MainWindow::on_actionSettings_triggered()
{

    if(settingsDialog==NULL)
        settingsDialog = new SettingsDialog();
    if(settingsDialog->getFormData(settings))
    {
        settings->writeAll();
        if(!settings->hotkey.isEmpty() && !logoffShortcut->setShortcut(QKeySequence(settings->hotkey)))
        {
            QMessageBox::critical(this,tr("错误"),tr("注销快捷键注册失败,快捷键无效或者可能已经被其他应用程序占用。"));
            this->logoffShortcut->setDisabled();
        }
    }
    delete settingsDialog;
    settingsDialog = NULL;
}

void MainWindow::hangedUp(bool natural)
{
    qDebug() << "hangedUp() enter";

    if(profile->open())
    {
        profile->setUserOnlineTime(this->username,allTime);

        profile->setMainWindowRect(this->username,
                                   this->pos().x(),
                                   this->pos().y(),
                                   /*this->size().width()*/0,
                                   /*this->size().height()*/0);

        profile->close();
    }
    
    if(natural)
    {
        if(this->app_exiting)
            qApp->exit(0);
        else
        {
            noticeDialog->hide();
            onStartLogining();
        }
    }else
    {
		onStopWorking();

        if(settings->autoRasdial)
        {
            noticeDialog->showMessage(tr("网络异常断开，正在重新拨号"));

            QEventLoop eventloop;
            QTimer::singleShot(10000, &eventloop, SLOT(quit()));
            eventloop.exec();

            if(!pppoe->redialRAS())
            {
                QMessageBox::critical(this,tr("提示"),tr("重新拨号失败"));
            }
        }else
        {
            //this->killTimer(timerId);
            Authenticat::getInstance()->endVerify();
            onStartLogining();
		//beginLoginUI();////////////////////
            QMessageBox::critical(loginDialog,tr("提示"),tr("网络异常断开。"));

        }

    }

    qDebug() << "hangedUp() exit";
    //if(app_exiting) qApp->quit();
}

void MainWindow::redialFinished(bool ok)
{
    qDebug() << "redialFinished() enter";
    if(ok)
    {
		onStartWorking();
		if(isMainWindowMinimized) hide();

        this->noticeDialog->showMessage(tr("拨号成功"));
        QEventLoop eventloop;
        QTimer::singleShot(500, &eventloop, SLOT(quit()));
        eventloop.exec();
        this->noticeDialog->hide();
        
        
        //this->logoffShortcut->setEnabled();
    }
    else
    {
        //this->killTimer(timerId);
        this->trayIcon->hide();
        this->hide();
        Authenticat::getInstance()->endVerify();
        noticeDialog->showMessage(tr("重播失败，正在关闭验证"));
        onStartLogining();
        //QMessageBox::information(this,tr("重播失败"),pppoe->lastError());
    }
    qDebug() << "redialFinished() exit";
}

void MainWindow::logoffShortcutActivated()
{
    this->ui->actionLogoff->trigger();
    qDebug() << "HOTKEY";
}

void MainWindow::on_actionActionFeedback_triggered()
{
	if(!feedbackDialog)
		feedbackDialog = new FeedbackDialog(this);
	feedbackDialog->setLoginData(pppoe->getUserName().trimmed(),"0");
	if(feedbackDialog->isVisible())
		feedbackDialog->activateWindow();
	else
		feedbackDialog->show();
}

void MainWindow::onStartWorking()
{
	//QMessageBox::information(this,"","onStartWorking");
	this->show();
	loginDialog->hide();
	trayIcon->setIcon(QIcon(":/icons/icons/tray_working.png"));
	trayIcon->setToolTip(tr("NDR 校园网络认证") + "\n" + pppoe->getIpAddress());

	trayIcon->setContextMenu(this->ui->menuTrayWorking);
	trayIcon->show();
    timerId = this->startTimer(1000*60);
    this->timerEvent(NULL);

	this->logoffShortcut->setEnabled();

	this->state = Working;
	ui->mainToolBar->show();
}

void MainWindow::onStopWorking()
{
	//QMessageBox::information(this,"","onStopWorking");
	this->state = Others;
	this->trayIcon->hide();
	this->hide();
	this->logoffShortcut->setDisabled();
	ui->mainToolBar->hide();
}

void MainWindow::onStartLogining()
{
	//QMessageBox::information(this,"","onStartLogining");
	this->hide();
	loginDialog->show();
	trayIcon->setIcon(QIcon(":/icons/icons/tray_login.png"));
	trayIcon->setToolTip(tr("NDR 校园网络认证"));

	trayIcon->setContextMenu(this->ui->menuTrayLogin);
	trayIcon->show();
	//trayIcon->de
	this->killTimer(timerId);

	this->state = Logining;
}

void MainWindow::onStopLogining()
{
	//QMessageBox::information(this,"","onStopLogining");
	this->state = Others;
	this->trayIcon->hide();
	loginDialog->hide();
}


void MainWindow::checkFinished(bool error,int major,int minor,QString errMsg)
{
    if(error && state == Working)
    {
        this->ui->actionLogoff->trigger();
        QMessageBox::critical(NULL,tr("警告"),tr("检查更新失败") + "\n" + errMsg);
        
    }

    /*  20160828 by cxh : use "!=" to easyly rollback
     *  Warning!!!:
     *      It Only Used at Non-developers Can Develop the New_NDR and Need to Rollback
     * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓This↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
     */
    if( major != VERSION_MAJOR || minor != VERSION_MINOR)
    {
        qDebug() << "需要更新！！！！！！！！！！！！";
        updateServer->downloadLatestPackage();
    }else
    {
        qDebug() << "不需要更新";
    }

}

void MainWindow::downloadFinished(bool error,QString errMsg)
{
    qDebug() << "downloadFinished() enter";
    if(error)
    {
        if(state==Working)
        {
            qDebug() << errMsg;
            ui->actionLogoff->trigger();
            QMessageBox::critical(loginDialog,"更新错误",tr("检查到新版本，但无法下载更新包") + "\n" + errMsg);
        }

        return;
    }else
    {
	HangupDialog hangupDialog;
	QString delayButtonText;
	QString acceptButtonText;
#if defined Q_OS_WIN || defined Q_OS_MAC
	delayButtonText = tr("稍后提醒");
	acceptButtonText = tr("立即安装");
	hangupDialog.setAcceptButtonText(acceptButtonText);
	hangupDialog.setDelayButtonText(delayButtonText);
	hangupDialog.setMessage(tr("NDR 将会在三分钟之内挂断并开启更新，如果您不想立即更新，请选择下次提示的时间并单击“%1”").arg(delayButtonText));
#elif defined Q_OS_LINUX
	delayButtonText = tr("稍后提醒");
	acceptButtonText = tr("打开软件包目录");
	hangupDialog.setAcceptButtonText(acceptButtonText);
	hangupDialog.setDelayButtonText(delayButtonText);
	hangupDialog.setMessage(tr("NDR 已经将新版本软件包下载到了临时目录，请打开目录并手动安装。如果您不想立即更新，请选择下次提示的时间并单击“%1”").arg(delayButtonText));
#endif
        hangupDialog.waitForUserAccept();
        if(app_exiting)
            return;
        QString errMsg1;
        if(updateServer->openPackage(errMsg1))
        {
            ui->actionQuit->trigger();
        }else{
            if(state == Working) ui->actionLogoff->trigger();
            QMessageBox::warning(loginDialog,
#ifdef Q_OS_LINUX
			tr("打开失败"), tr("打开目录失败")
#else
			tr("安装失败"), tr("启动安装程序失败")
#endif
			+ "\n" + errMsg1 + "\n" +
			tr("请尝试手动安装 %1").arg(updateServer->packagePath()));
        }
        //QMessageBox::information(0,"提示","打开文件");
    }

}

void MainWindow::loginWindowClosed()
{
    app_exiting=true;
}
