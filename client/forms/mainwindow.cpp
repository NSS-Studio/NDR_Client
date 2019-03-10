#include "mainwindow.hpp"
#include "logindialog.hpp"
#include "ndrapplication.hpp"
#include "ui_mainwindow.h"
#include "utils.hpp"
#include <QDesktopServices>
#include <QMessageBox>
#include <QScreen>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow} {
    this->setWindowFlags(this->windowFlags() | Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());

    auto pppoe = utils::resourceManager.getPPPoE();
    auto loginDialog = utils::resourceManager.getLoginDialog();

    ui->menuTrayLogin->menuAction()->setVisible(false);
    ui->menuTrayWorking->menuAction()->setVisible(false);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "System tray is not available, tray icon disabled";
    } else {
        trayIcon.reset(new QSystemTrayIcon{});
        this->trayIcon->show();
        connect(trayIcon.get(), &QSystemTrayIcon::activated, this,
                &MainWindow::iconActivated);
    }

    // Init PPPoE singals&slots
    //  connect(pppoe, &PPPoE::dialFinished, this, &MainWindow::dialFinished,
    //          Qt::QueuedConnection);
    connect(pppoe, &PPPoE::hangedUp, this, &MainWindow::hangedUp,
            Qt::QueuedConnection);

#ifndef Q_OS_WIN
    QStringList interfaces = pppoe->getAvailableInterfaces();
    if (interfaces.count() == 0) {
        QMessageBox::critical(this, tr("NDR"), tr("No Interface Available"));
        qApp->exit(1);
        loginDialog->close();
        close();
        exit(1);
    }
    loginDialog->set_interface_list(interfaces);
#endif
    connect(loginDialog, &LoginDialog::myaccepted, this, &MainWindow::tryLogin);
    connect(loginDialog, &LoginDialog::finished, this,
            &MainWindow::loginWindowClosed);

    noticeDialog.reset(new NoticeDialog{});

    connect(this, &MainWindow::minimumWindow, this, &MainWindow::hide,
            Qt::QueuedConnection); //绑定最小化到隐藏
    isMainWindowMinimized = false;

    //更新模块初始化
    updateServer.reset(new UpdateService{NDR_UPDATE_SERVER, utils::tempDir});

    connect(updateServer.get(), &UpdateService::checkFinished, this,
            &MainWindow::checkFinished);
    connect(updateServer.get(), &UpdateService::downloadFinished, this,
            &MainWindow::downloadFinished);

    this->ui->lblAllTime->setText("NUL");

    settingsDialog = nullptr;
    feedbackDialog = nullptr;
    app_exiting = false;

    onStartLogining();
}

MainWindow::~MainWindow() { trayIcon->hide(); }

void MainWindow::tryLogin() {
    QString username, password, deviceName, errorMessage, realUsername, postfix;
    onStopLogining();
    auto loginDialog = utils::resourceManager.getLoginDialog();
    loginDialog->getFormData(username, password, postfix, deviceName);
    this->ui->lblAccount->setText(username);

    auto modelCaption = utils::getDrModelCaption(postfix);
    if (modelCaption.isValid())
        this->ui->lblType->setText(modelCaption.toString());
    else
        this->ui->lblType->setText(tr("未知"));
    qDebug() << "PPPoE Account: " << username + postfix;
    realUsername = "\r\n" + username + postfix;
    noticeDialog->showMessage(tr("正在拨号. . ."));
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->dialRAS(NDR_PHONEBOOK_NAME, realUsername, password, deviceName);
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
    auto profile = utils::resourceManager.getProfile();
    auto loginDialog = utils::resourceManager.getLoginDialog();
    auto pppoe = utils::resourceManager.getPPPoE();
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
            QSettings conn_cfg(utils::appHome + "/connection.cfg",
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
        noticeDialog->close();

        if (ENABLE_UPDATE)
            updateServer->checkUpdate();
        if (!utils::settings->webUpEnable)
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
    info->checkInfoGet();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
#ifndef Q_OS_MAC
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        on_actionShowWindow_triggered();
        break;
    default:
        break;
    }
#else
    // warning: unused parameter 'reason' [-Wunused-parameter]
    Q_UNUSED(reason);
#endif
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (utils::settings->quitWhileCloseWindow) {
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
    if (state == State::Working) {
        app_exiting = true;
        this->ui->actionLogoff->trigger();
    } else {
        qApp->exit(0);
    }
}

void MainWindow::on_actionShowWindow_triggered() {
    auto loginDialog = utils::resourceManager.getLoginDialog();
    if (state == State::Working) {
        this->show();
        this->activateWindow();
        isMainWindowMinimized = false;
    } else if (state == State::Logining) {
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
    qDebug() << "mainwindow thread: " << QThread::currentThread() << endl;
    noticeDialog->showMessage(tr("正在尝试注销"));

    QEventLoop loop;
    QTimer::singleShot(100, &loop, SLOT(quit()));
    loop.exec();
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->hangUp();
    qDebug() << "this->pppoe->hangUp();" << endl;
    onStopWorking();
    qDebug() << "onStopWorking();" << endl;
}

void MainWindow::on_actionAbout_triggered() {
    auto aboutDialog = utils::resourceManager.getAboutDialog();
    if (aboutDialog->isVisible())
        aboutDialog->activateWindow();
    else
        aboutDialog->show();
}

void MainWindow::verifyStoped() {
    qDebug() << "verifyStoped() enter";
    {
        noticeDialog->hide(); //重播失败，正在关闭验证
        if (!this->app_exiting) {
            // onStartLogining();
            // beginLoginUI();
        }
    }
    qDebug() << "verifyStoped() exit";
}

void MainWindow::timerEvent(QTimerEvent *) {
    static int timepassed = 0;
    static int time = 0;
    this->ui->lblTime->setText(time_humanable(timepassed));

    // server return minute
    this->ui->lblAllTime->setText(time_humanable(time * 60 + timepassed % 60));
    // time ++ ;

    //    auto popUpInfomationDialog =
    //    utils::resourceManager.getPopUpInfomationDialog();

    //    if (!(timepassed % 300))
    //        popUpInfomationDialog->getXmlFromNSS(NDR_POPUP_URL);
    timepassed += 1;
}

void MainWindow::on_actionSettings_triggered() {
    if (settingsDialog.isNull())
        settingsDialog.reset(new SettingsDialog{});
    if (settingsDialog->getFormData(utils::settings.get())) {
        utils::settings->writeAll();
    }
}

void MainWindow::hangedUp(bool natural) {
    qDebug() << "hangedUp() enter";
    auto profile = utils::resourceManager.getProfile();
    auto pppoe = utils::resourceManager.getPPPoE();
    auto loginDialog = utils::resourceManager.getLoginDialog();
    if (profile->open()) {
        profile->setUserOnlineTime(this->username, allTime);

        profile->setMainWindowRect(this->username, this->pos().x(),
                                   this->pos().y(),
                                   /*this->size().width()*/ 0,
                                   /*this->size().height()*/ 0);

        profile->close();
    }

    if (natural) {
        if (this->app_exiting) {
            qApp->exit(0);
        } else {
            noticeDialog->hide();
            onStartLogining();
        }
    } else {
        onStopWorking();

        if (utils::settings->autoRasdial) {
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
            onStartLogining();
            QMessageBox::critical(loginDialog, tr("提示"),
                                  tr("网络异常断开。"));
        }
    }

    qDebug() << "hangedUp() exit";
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
    } else {
        // this->killTimer(timerId);
        this->trayIcon->hide();
        this->hide();
        noticeDialog->showMessage(tr("重播失败，正在关闭验证"));
        onStartLogining();
        // QMessageBox::information(this,tr("重播失败"),pppoe->lastError());
    }
    qDebug() << "redialFinished() exit";
}

void MainWindow::on_actionActionFeedback_triggered() {
    if (!feedbackDialog)
        feedbackDialog.reset(new FeedbackDialog{});
    auto pppoe = utils::resourceManager.getPPPoE();
    feedbackDialog->setLoginData(pppoe->getUserName().trimmed(), "0");
    if (feedbackDialog->isVisible())
        feedbackDialog->activateWindow();
    else
        feedbackDialog->show();
}

void MainWindow::onStartWorking() {
    qDebug() << "Timer creat" << endl;
    auto loginDialog = utils::resourceManager.getLoginDialog();
    auto pppoe = utils::resourceManager.getPPPoE();
    this->show();
    loginDialog->hide();
    trayIcon->setIcon(QIcon(":/icons/icons/tray_working.png"));
    trayIcon->setToolTip(tr("NDR 校园网络认证") + "\n" + pppoe->getIpAddress());

    trayIcon->setContextMenu(this->ui->menuTrayWorking);
    trayIcon->show();
    timerId = this->startTimer(1000);
    this->timerEvent(nullptr);

    //	this->logoffShortcut->setEnabled();

    this->state = State::Working;
    ui->mainToolBar->show();
}

void MainWindow::onStopWorking() {
    qDebug() << "Timer delete" << endl;
    this->state = State::Others;
    this->trayIcon->hide();
    this->hide();
    ui->mainToolBar->hide();
    this->killTimer(timerId);
}

void MainWindow::onStartLogining() {
    qDebug() << "Function: onStartLogining" << endl;
    // QMessageBox::information(this,"","onStartLogining");
    this->hide();
    auto loginDialog = utils::resourceManager.getLoginDialog();
    loginDialog->show();
    trayIcon->setIcon(QIcon(":/icons/icons/tray_login.png"));
    trayIcon->setToolTip(tr("NDR 校园网络认证"));

    trayIcon->setContextMenu(this->ui->menuTrayLogin);
    trayIcon->show();

    this->state = State::Logining;
}

void MainWindow::onStopLogining() {
    auto loginDialog = utils::resourceManager.getLoginDialog();
    this->state = State::Others;
    this->trayIcon->hide();
    loginDialog->hide();
}

void MainWindow::checkFinished(bool error, int major, int minor,
                               QString const &errMsg) {
    if (error && state == State::Working) {
        this->ui->actionLogoff->trigger();
        QMessageBox::critical(nullptr, tr("警告"),
                              tr("检查更新失败") + "\n" + errMsg);
    }

    auto ndrVersion = major * 100 + minor;

    if (ndrVersion > NDR_VERSION) {
        qDebug() << "需要更新！！！！！！！！！！！！";
        updateServer->downloadLatestPackage();
    } else {
        qDebug() << "不需要更新";
    }
}

void MainWindow::downloadFinished(bool error, QString const &errMsg) {
    qDebug() << "downloadFinished() enter";
    auto loginDialog = utils::resourceManager.getLoginDialog();
    if (error) {
        if (state == State::Working) {
            qDebug() << errMsg;
            ui->actionLogoff->trigger();
            QMessageBox::critical(loginDialog, "更新错误",
                                  tr("检查到新版本，但无法下载更新包") + "\n" +
                                      errMsg);
        }
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
            if (state == State::Working)
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
    }
}

void MainWindow::loginWindowClosed() { app_exiting = true; }

void MainWindow::on_goDnuiBrowser_clicked() {
    ui->goDnuiBrowser->setEnabled(false);
    QUrl web{QString{NDR_GATE}};
    QDesktopServices::openUrl(web);
    ui->goDnuiBrowser->setEnabled(true);
}
