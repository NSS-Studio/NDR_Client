#include "mainwindow.h"
#include "infomodule.h"
#include "logindialog.h"
#include "ndrapplication.hpp"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QScreen>
#include <QUrl>
/*
#ifdef Q_OS_MAC
#include <QtGui/QMacStyle>
#endif
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    // this->setWindowFlags(Qt::Dialog);
    this->setWindowFlags(this->windowFlags() | Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);
    // int x, y;
    // getMoniterSize(x, y);
    // this -> resize(QSize(x/6.2, y/3));

    // settings = new SettingsSet(appHome + "/config.ini");

    popUp = new popUpDialog();

    profile = new LocalStorage(
        appHome + "/config.db"); //如果数据库结构变化，修改文件名抛弃数据

    this->ui->menuTrayLogin->menuAction()->setVisible(false);
    this->ui->menuTrayWorking->menuAction()->setVisible(false);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        trayIcon = nullptr;
        qDebug() << "System tray is not available, tray icon disabled";
        return;
    }

    trayIcon = new QSystemTrayIcon();
    this->trayIcon->show();
    connect(trayIcon, &QSystemTrayIcon::activated, this,
            &MainWindow::iconActivated);

    this->pppoe = new PPPoE();
    connect(this->pppoe, SIGNAL(dialFinished(bool)), this,
            SLOT(dialFinished(bool)), Qt::QueuedConnection);
    connect(this->pppoe, SIGNAL(hangedUp(bool)), this, SLOT(hangedUp(bool)),
            Qt::QueuedConnection);

    this->loginDialog = new LoginDialog(profile);

#ifndef Q_OS_WIN
    QStringList interfaces = PPPoE::getAvailableInterfaces();
    if (interfaces.count() == 0) {
        QMessageBox::critical(this, tr("NDR"), tr("No Interface Available"));
        QApplication::instance()->exit(1);
        loginDialog->close();
        delete loginDialog;
        close();
//        static_cast<SingleApplication*>(QApplication::instance())->releaseSharedMemory();
        exit(1);
    }
    this->loginDialog->set_interface_list(interfaces);
#endif
    connect(this->loginDialog, SIGNAL(myaccepted()), this, SLOT(tryLogin()));
    connect(this->loginDialog, SIGNAL(finished(int)), this,
            SLOT(loginWindowClosed()));

    // forget to delete this object, add to mainWindow's child to autoDelete
    this->noticeDialog = new NoticeDialog(this);

//    connect(Authenticat::getInstance(), SIGNAL(verifyStoped()), this,
//            SLOT(verifyStoped()), Qt::QueuedConnection);

    connect(this, SIGNAL(minimumWindow()), this, SLOT(hide()),
            Qt::QueuedConnection); //绑定最小化到隐藏

    isMainWindowMinimized = false;

    //更新模块初始化
    updateServer = new UpdateService(NDR_UPDATE_SERVER, tempDir);

    connect(updateServer, &UpdateService::checkFinished, this,
            &MainWindow::checkFinished);
    connect(updateServer, &UpdateService::downloadFinished, this,
            &MainWindow::downloadFinished);

    this->ui->lblAllTime->setText("NULL");
    this->ui->lblFlow->setText("NULL");

    aboutDialog = nullptr;
    settingsDialog = nullptr;
    feedbackDialog = nullptr;
    app_exiting = false;

    onStartLogining();

    /***/
    // updateServer->checkUpdate();
    //最后信息获取按钮的信号连接
    connect(ui->actionGetInfo, &QAction::triggered, this,
            &MainWindow::getSystemInfo);
    connect(this, &MainWindow::infoWriteStarted,
            InfoModuleThread::getInstance(),
            &InfoModuleThread::startGetInfoToWriteFile);
}

MainWindow::~MainWindow() {
    // can not kill timer at this function, the time counter will crash
    // this->killTimer(timerId);

    if (noticeDialog != nullptr)
        delete noticeDialog;

    if (updateServer)
        delete updateServer;

    delete loginDialog;
    if (aboutDialog)
        delete aboutDialog;
    if (settingsDialog)
        delete settingsDialog;

    if (feedbackDialog)
        delete feedbackDialog;

    delete pppoe;
    trayIcon->hide();
    delete trayIcon;
    delete ui;

    delete profile;
    // delete logoffShortcut;
    delete popUp;
    // delete settings;
}

void MainWindow::tryLogin() {
    QString username, password, device_name, errorMessage;
    QString realUsername;
    QString postfix;
    onStopLogining();
    loginDialog->getFormData(username, password, postfix, device_name);
    this->ui->lblAccount->setText(username);

    // h
    QString model_caption;
    if (__getDrModelCaption(postfix, model_caption))
        this->ui->lblType->setText(model_caption);
    else
        this->ui->lblType->setText(tr("未知"));
    qDebug() << "Function: " << __PRETTY_FUNCTION__;
    qDebug() << "PPPoE Account: " << username + postfix;
    realUsername = "\r\n" + username + postfix; //+ "@student";
    //    realUsername=username;//+ "@student";

    noticeDialog->showMessage(tr("正在拨号. . ."));
    pppoe->dialRAS(NDR_PHONEBOOK_NAME, realUsername, password, device_name);

    // TODO delete

    networkInterfaceCardName = device_name;

    // this->show();
    // Authenticat::getInstance()->beginVerify(DRCOM_SERVER_IP,DRCOM_SERVER_PORT);
}

QString MainWindow::time_humanable(int sec) {
    int day, hour, minute, second;
    int t;

    t = 24 * 3600;
    day = sec / t;

    sec %= t;
    t = 3600;
    hour = sec / t;

    sec %= t;
    t = 60;
    minute = sec / t;

    sec %= t;
    second = sec;

    return tr("%0 天 %1:%2:%3")
        .arg(day, -3, 10, QChar(' '))
        .arg(hour, 2, 10, QChar('0'))
        .arg(minute, 2, 10, QChar('0'))
        .arg(second, 2, 10, QChar('0'));
}

void MainWindow::dialFinished(bool ok) {
    qDebug() << QString("dialFinished(%1) enter").arg(ok);
    if (ok) {
        if (profile->open()) {
            QString username, password, device_name;
            QString manner;
            bool autoLogin, savePassword;
            loginDialog->getFormData(username, password, manner, device_name,
                                     &autoLogin, &savePassword);
            profile->setLoginInfo(username, savePassword ? password : "",
                                  manner);
            profile->setDeviceName(device_name); // Deprecated
            QSettings conn_cfg(appHome + "/connection.cfg",
                               QSettings::IniFormat);
            conn_cfg.setValue("Interface/Etherface", device_name);
            this->username = username;
            int left, top, width, height;

            QScreen *screen = QGuiApplication::primaryScreen();
            QRect mm = screen->availableGeometry();
            int desktop_width = mm.width();
            int desktop_height = mm.height();
            if (profile->getMainWindowRect(username, left, top, width,
                                           height)) {

                if (left + this->width() > desktop_width - 100 ||
                    left + this->width() < 100 ||
                    top + this->height() > desktop_height - 100 ||
                    top + this->height() < 100) {
                    left = desktop_width - this->width() - 100;
                    top = (desktop_height - this->height()) / 2;
                }
                // QMessageBox::information(this,"",QString::number(width)+ " "
                // + QString::number(height));
                this->move(left, top);
                // this->resize(width,height);
            } else {
                left = desktop_width - this->width() - 100;
                top = (desktop_height - this->height()) / 2;
                this->move(left, top);
            }

            if (!profile->getUserOnlineTime(username, allTime)) {
                this->allTime = 0;
            }

            this->ui->lblAllTime->setText(time_humanable(this->allTime));

            if (autoLogin)
                profile->setAutoLoginUser(username);
            else {
                QString autoLoginUserName;
                if (profile->getAutoLoginUser(autoLoginUserName) &&
                    autoLoginUserName == username)
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

        //! 废除自动最小化 因为上了皮皮GO！！！！！！！！！！！
        //        if(settings->autoMinimize)
        //        {
        //            this->isMainWindowMinimized=true;
        //            this->hide();
        //            this->trayIcon->showMessage(tr("NDR
        //            校园网络认证"),tr("主面板已最小化到这里，您可以进入设置关闭自动最小化功能。"),QSystemTrayIcon::Information,4000);
        //        }
        noticeDialog->close();
//        Authenticat::getInstance()->beginVerify(
//            DRCOM_SERVER_IP, DRCOM_SERVER_PORT); //必须在beginworkingui前

        // verifyEncrypt();//必须在beginworkingui前
        if (ENABLE_UPDATE)
            updateServer->checkUpdate();
        // getInfoAboutNss
        if (!settings->webUpEnable)
            getMessageFromNSS();

    } else {
        noticeDialog->hide();
        onStartLogining();
#ifdef Q_OS_MAC
        if (!pppoe->lastError().isEmpty())
#endif
            QMessageBox::information(loginDialog, tr("提示"),
                                     tr("拨号失败") + "\n" +
                                         pppoe->lastError());
    }

    qDebug() << "dialFinished() exit";
}

void MainWindow::getMessageFromNSS() {
    GetInfoAboutNSS *info = GetInfoAboutNSS::getInstance();
    //    connect(info, &GetInfoAboutNSS::endGetInfo, info,
    //    &GetInfoAboutNSS::deleteLater);
    info->checkInfoGet();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
#ifndef Q_OS_MAC
    switch (reason) {
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
#else
    // warning: unused parameter 'reason' [-Wunused-parameter]
    UNUSED(reason);
#endif
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (settings->quitWhileCloseWindow) {
        this->ui->actionLogoff->trigger();
        app_exiting = true;
    } else {
        this->hide();
    }
    event->ignore();
}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState() & Qt::WindowMinimized) {
            emit minimumWindow();
            isMainWindowMinimized = true;
            // this->showNormal();
            // event->ignore();
        }

        QMainWindow::changeEvent(event);
        // this->QMainWindow::changeEvent(event);
    }
}

void MainWindow::on_actionQuit_triggered() {
    if (state == Working) {
        app_exiting = true;
        this->ui->actionLogoff->trigger();
        return;
    } else {
        qApp->exit(0);
    }
}

void MainWindow::on_actionShowWindow_triggered() {
    //奇怪
    if (state == Working) {
        this->show();
        this->activateWindow();
        isMainWindowMinimized = false;
    } else if (state == Logining) {
        // loginDialog->showNormal();
        loginDialog->show();
        loginDialog->activateWindow();
    }
}

void MainWindow::on_actionShowLoginDialog_triggered() {
    /*
    if(state == Logining) {
        loginDialog->showNormal();
        loginDialog->activateWindow();
    }*/
}

void MainWindow::on_actionLogoff_triggered() {
    qDebug("slot: on_actionLogoff_triggered()");
    qDebug() << "mainwindow thread: " << QThread::currentThread() << endl;
    noticeDialog->showMessage(tr("正在尝试注销"));

    QEventLoop loop;
    QTimer::singleShot(100, &loop, SLOT(quit()));
    loop.exec();

//    Authenticat::getInstance()->endVerify();
    qDebug() << "Authenticat::getInstance()->endVerify();" << endl;
    this->pppoe->hangUp();
    qDebug() << "this->pppoe->hangUp();" << endl;
    onStopWorking();
    qDebug() << "onStopWorking();" << endl;
}

void MainWindow::on_actionAbout_triggered() {
    if (aboutDialog == nullptr)
        aboutDialog = new AboutDialog();
    if (aboutDialog->isVisible())
        aboutDialog->activateWindow();
    else
        aboutDialog->show();
}

void MainWindow::verifyStoped() {
    qDebug() << "verifyStoped() enter";
    {
        // trayIcon->show();
        noticeDialog->hide(); //重播失败，正在关闭验证
        if (!this->app_exiting) {

            // onStartLogining();
            // beginLoginUI();
        }
    }
    qDebug() << "verifyStoped() exit";
}

static bool _get_account_state(int *time, int *flow) {
    Q_ASSERT(time);
    Q_ASSERT(flow);
    QNetworkAccessManager nam;
    QNetworkReply *reply;
    QEventLoop eventloop;

    reply = nam.get(QNetworkRequest(QUrl("http://172.24.253.35/")));
    QObject::connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    QTimer::singleShot(10 * 1000, &eventloop, SLOT(quit()));
    eventloop.exec();
    // this->ui->textBrowser->setText(reply->readAll());
    if (!reply->isFinished()) {
        reply->abort();
        qDebug() << "_get_account_state"
                 << "http request timeout";
        return false;
    }
    QString stateValue =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();

    if (stateValue != "200") {
        qDebug() << "_get_account_state"
                 << "stauts code not 200.";
        return false;
    }
    QString html = reply->readAll();
    QRegExp regexp("time='(\\d+)[ ]*';flow='(\\d+)[ ]*';");
    int pos = html.indexOf(regexp);
    if (pos >= 0) {
        qDebug() << "matched string ->" << regexp.capturedTexts();
        *time = regexp.cap(1).toInt();
        *flow = regexp.cap(2).toInt();
        qDebug() << "time:" << *time;
        qDebug() << "flow:" << *flow;
        return true;
    } else {
        qDebug() << "_get_account_state"
                 << "unmatched.";
        return false;
    }
}
void MainWindow::timerEvent(QTimerEvent *) {
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
    static int time = 0;
    int flow;
    if (!(timepassed % 60) && _get_account_state(&time, &flow)) {
        this->ui->lblFlow->setText(
            QString("%1 KB").arg(flow, -3, 10, QChar(' ')));
    }
    this->ui->lblTime->setText(time_humanable(timepassed));

    // server return minute
    this->ui->lblAllTime->setText(time_humanable(time * 60 + timepassed % 60));
    // time ++ ;

    if (!(timepassed % 300))
        popUp->getXmlFromNSS(NDR_POPUP_URL);
    timepassed += 1;
}

void MainWindow::on_actionSettings_triggered() {

    if (settingsDialog == nullptr)
        settingsDialog = new SettingsDialog();
    if (settingsDialog->getFormData(settings)) {
        settings->writeAll();
        // if(!settings->hotkey.isEmpty() &&
        // !logoffShortcut->setShortcut(QKeySequence(settings->hotkey)))
        //{
        //    QMessageBox::critical(this,tr("错误"),tr("注销快捷键注册失败,快捷键无效或者可能已经被其他应用程序占用。"));
        //    this->logoffShortcut->setDisabled();
        //}
    }
    //    delete settingsDialog;
    //    settingsDialog = nullptr;
}

void MainWindow::hangedUp(bool natural) {
    qDebug() << "hangedUp() enter";

    if (profile->open()) {
        profile->setUserOnlineTime(this->username, allTime);

        profile->setMainWindowRect(this->username, this->pos().x(),
                                   this->pos().y(),
                                   /*this->size().width()*/ 0,
                                   /*this->size().height()*/ 0);

        profile->close();
    }

    if (natural) {
        if (this->app_exiting)
            qApp->exit(0);
        else {
            noticeDialog->hide();
            onStartLogining();
        }
    } else {
        onStopWorking();

        if (settings->autoRasdial) {
            // QEventLoop eventloop;
            // QTimer::singleShot(10000, &eventloop, SLOT(quit()));
            // eventloop.exec();

            for (int retryCount = 1; retryCount <= 5 && pppoe->isDisconnect();
                 retryCount++) {
                QThread::sleep(1000);
                noticeDialog->showMessage(
                    tr("网络异常断开，正在重新拨号\n重试次数%0/5")
                        .arg(retryCount));
                QEventLoop eventloop;
                QTimer::singleShot(500, &eventloop, SLOT(quit()));
                eventloop.exec();
                pppoe->redialRAS();
            }

            if (pppoe->isDisconnect())
                emit redialFinished(false);
            else
                emit redialFinished(true);
        } else {
            // this->killTimer(timerId);
//            Authenticat::getInstance()->endVerify();
            onStartLogining();
            // beginLoginUI();////////////////////
            QMessageBox::critical(loginDialog, tr("提示"),
                                  tr("网络异常断开。"));
        }
    }

    qDebug() << "hangedUp() exit";
    // if(app_exiting) qApp->quit();
}

void MainWindow::redialFinished(bool ok) {
    qDebug() << "redialFinished() enter";
    if (ok) {
        onStartWorking();
        if (isMainWindowMinimized)
            hide();

        this->noticeDialog->showMessage(tr("拨号成功"));
        QEventLoop eventloop;
        QTimer::singleShot(500, &eventloop, SLOT(quit()));
        eventloop.exec();
        this->noticeDialog->hide();

        // this->logoffShortcut->setEnabled();
    } else {
        // this->killTimer(timerId);
        this->trayIcon->hide();
        this->hide();
//        Authenticat::getInstance()->endVerify();
        noticeDialog->showMessage(tr("重播失败，正在关闭验证"));
        onStartLogining();
        // QMessageBox::information(this,tr("重播失败"),pppoe->lastError());
    }
    qDebug() << "redialFinished() exit";
}

// void MainWindow::logoffShortcutActivated()
//{
//    this->ui->actionLogoff->trigger();
//    qDebug() << "HOTKEY";
//}

void MainWindow::on_actionActionFeedback_triggered() {
    if (!feedbackDialog)
        feedbackDialog = new FeedbackDialog(this);
    feedbackDialog->setLoginData(pppoe->getUserName().trimmed(), "0");
    if (feedbackDialog->isVisible())
        feedbackDialog->activateWindow();
    else
        feedbackDialog->show();
}

void MainWindow::onStartWorking() {
    qDebug() << "Timer creat" << endl;
    // QMessageBox::information(this,"","onStartWorking");

    this->show();
    loginDialog->hide();
    trayIcon->setIcon(QIcon(":/icons/icons/tray_working.png"));
    trayIcon->setToolTip(tr("NDR 校园网络认证") + "\n" + pppoe->getIpAddress());

    trayIcon->setContextMenu(this->ui->menuTrayWorking);
    trayIcon->show();
    timerId = this->startTimer(1000);
    this->timerEvent(nullptr);

    //	this->logoffShortcut->setEnabled();

    this->state = Working;
    ui->mainToolBar->show();
}

void MainWindow::onStopWorking() {
    qDebug() << "Timer delete" << endl;
    this->state = Others;
    this->trayIcon->hide();
    this->hide();
    ui->mainToolBar->hide();
    this->killTimer(timerId);
}

void MainWindow::onStartLogining() {
    qDebug() << "Function: onStartLogining" << endl;
    // QMessageBox::information(this,"","onStartLogining");
    this->hide();
    loginDialog->show();
    trayIcon->setIcon(QIcon(":/icons/icons/tray_login.png"));
    trayIcon->setToolTip(tr("NDR 校园网络认证"));

    trayIcon->setContextMenu(this->ui->menuTrayLogin);
    trayIcon->show();
    // trayIcon->de
    // this->kill Timer(timerId); //timer kill when it didn't brith

    this->state = Logining;
}

void MainWindow::onStopLogining() {
    // QMessageBox::information(this,"","onStopLogining");
    this->state = Others;
    this->trayIcon->hide();
    loginDialog->hide();
}

void MainWindow::checkFinished(bool error, int major, int minor,
                               QString errMsg) {
    if (error && state == Working) {
        this->ui->actionLogoff->trigger();
        QMessageBox::critical(nullptr, tr("警告"),
                              tr("检查更新失败") + "\n" + errMsg);
    }

    /*  20160828 by cxh : use "!=" to easyly rollback
     *  Warning!!!:
     *      It Only Used at Non-developers Can Develop the New_NDR and Need to
     * Rollback ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓This↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
     */
    if (major > VERSION_MAJOR || minor > VERSION_MINOR) {
        qDebug() << "需要更新！！！！！！！！！！！！";
        updateServer->downloadLatestPackage();
    } else {
        qDebug() << "不需要更新";
    }
}

void MainWindow::downloadFinished(bool error, QString errMsg) {
    qDebug() << "downloadFinished() enter";
    if (error) {
        if (state == Working) {
            qDebug() << errMsg;
            ui->actionLogoff->trigger();
            QMessageBox::critical(loginDialog, "更新错误",
                                  tr("检查到新版本，但无法下载更新包") + "\n" +
                                      errMsg);
        }

        return;
    } else {
        HangupDialog hangupDialog;
        QString delayButtonText;
        QString acceptButtonText;
#if defined Q_OS_WIN || defined Q_OS_MAC
        delayButtonText = tr("稍后提醒");
        acceptButtonText = tr("立即安装");
        hangupDialog.setAcceptButtonText(acceptButtonText);
        hangupDialog.setDelayButtonText(delayButtonText);
        hangupDialog.setMessage(tr("NDR "
                                   "将会在三分钟之内挂断并开启更新，如果您不想"
                                   "立即更新，请选择下次提示的时间并单击“%1”")
                                    .arg(delayButtonText));
#elif defined Q_OS_LINUX
        delayButtonText = tr("稍后提醒");
        acceptButtonText = tr("打开软件包目录");
        hangupDialog.setAcceptButtonText(acceptButtonText);
        hangupDialog.setDelayButtonText(delayButtonText);
        hangupDialog.setMessage(
            tr("NDR "
               "已经将新版本软件包下载到了临时目录，请打开目录并手动安装。如果"
               "您不想立即更新，请选择下次提示的时间并单击“%1”")
                .arg(delayButtonText));
#endif
        hangupDialog.waitForUserAccept();
        if (app_exiting)
            return;
        QString errMsg1;
        if (updateServer->openPackage(errMsg1)) {
            ui->actionQuit->trigger();
        } else {
            if (state == Working)
                ui->actionLogoff->trigger();
            QMessageBox::warning(
                loginDialog,
#ifdef Q_OS_LINUX
                tr("打开失败"),
                tr("打开目录失败")
#else
                tr("安装失败"),
                tr("启动安装程序失败")
#endif
                    + "\n" + errMsg1 + "\n" +
                    tr("请尝试手动安装 %1").arg(updateServer->packagePath()));
        }
        // QMessageBox::information(0,"提示","打开文件");
    }
}

void MainWindow::loginWindowClosed() { app_exiting = true; }

void MainWindow::on_goDnuiBrowser_clicked() {
    ui->goDnuiBrowser->setEnabled(false);
    QUrl web(QString(NDR_GATE));
    QDesktopServices::openUrl(web);
    ui->goDnuiBrowser->setEnabled(true);
}

void MainWindow::getSystemInfo() {
    qDebug() << "call infoGet clicked";
    // too ugly ↓↓↓↓
    // QDialog *message = new QDialog(this, Qt::WindowMinimizeButtonHint);
    // QLabel *label = new QLabel(message);
    // QHBoxLayout *lay = new QHBoxLayout();
    //
    // lay->addWidget(label);
    // message->setLayout(lay);
    // message->setFixedSize(322, 80);
    //
    // label->setFrameStyle(QFrame::StyledPanel);
    // label->setText("现在程序正在获取信息，请耐心等候.\n"
    //               "当信息获取完毕时本对话框会自动关闭.\n"
    //               "信息文件将会保存在您的桌面，文件名为：Info.txt");
    // message->setWindowTitle(tr("正在处理..."));
    // message->setWindowIcon(QIcon(tr("/icon/about.png")));

    noticeDialog->showMessage("现在程序正在获取信息，请耐心等候.\n"
                              "当信息获取完毕时本对话框会自动关闭.\n"
                              "信息文件将会保存在您的桌面，文件名为：Info.txt");

    connect(InfoModuleThread::getInstance(), &InfoModuleThread::infoGetFinished,
            noticeDialog, &NoticeDialog::close);

    emit infoWriteStarted();

}

