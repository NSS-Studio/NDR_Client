#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "common.h"
#include "noticedialog.h"
#include "logindialog.h"
#include "authenticat.h"
#include "pppoe.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "localstorage.h"
#include "qxtglobalshortcut.h"
#include "feedbackdialog.h"
#include "updateservice.h"
#include "hangupdialog.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <information/userinfo.h>
#include <information/usrinfohttppost.h>
#include <information/interfaceinfo.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum State{
        Logining,
        Working,
        Others
    };

private slots:
	void on_actionQuit_triggered();
	void on_actionShowWindow_triggered();
	void on_actionShowLoginDialog_triggered();
	void on_actionLogoff_triggered();
	void on_actionAbout_triggered();
	void on_actionSettings_triggered();
	void hangedUp(bool natural);
	void tryLogin();
	void dialFinished(bool ok);
	void verifyStoped();
	void redialFinished(bool ok);
	void on_actionActionFeedback_triggered();
	void onStartWorking();
	void onStopWorking();
	void onStartLogining();
	void onStopLogining();
    void checkFinished(bool error,int major,int minor,QString errMsg);
    void downloadFinished(bool error,QString errMsg);
    void loginWindowClosed();
private:
    Ui::MainWindow *ui;
    PPPoE *pppoe;
    LoginDialog *loginDialog;
    NoticeDialog *noticeDialog;
    SettingsDialog *settingsDialog;
    AboutDialog *aboutDialog;
    FeedbackDialog *feedbackDialog;
    QSystemTrayIcon *trayIcon;
    QMenu *myMenu;  
    
    QxtGlobalShortcut * logoffShortcut;
    
    State state;
    bool app_exiting; //退出标记
    bool isMainWindowMinimized;
    
    LocalStorage *profile;
    int connTime;
    int allTime;
    QString username;
    int timerId;
    
    //SettingsSet *settings;
    UpdateService *updateServer;
    InterfaceInfo *interfaceInfo;
    void timerEvent(QTimerEvent *);
    void changeEvent(QEvent* event);
    
    QString time_humanable(int sec);
    

public slots:  
    void iconActivated(QSystemTrayIcon::ActivationReason reason);  
    void logoffShortcutActivated();
protected:  
    void closeEvent(QCloseEvent *event);  
Q_SIGNALS:
    void minimumWindow();
};

#endif // MAINWINDOW_H
