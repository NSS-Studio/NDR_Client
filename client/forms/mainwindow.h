#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "aboutdialog.h"
#include "localstorage.h"
#include "logindialog.h"
#include "noticedialog.h"
#include <pppoe.hpp>
#include "settingsdialog.h"
#include "feedbackdialog.h"
#include "getinfoaboutnss.h"
#include "hangupdialog.h"
#include "popupdialog.h"
#include "updateservice.h"
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <ndrapplication.hpp>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QSharedPointer<NdrApplication> app, QWidget *parent = nullptr);
    ~MainWindow();
    enum State { Logining, Working, Others };

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
    void checkFinished(bool error, int major, int minor, QString errMsg);
    void downloadFinished(bool error, QString errMsg);
    void loginWindowClosed();
    void getMessageFromNSS();

    void on_goDnuiBrowser_clicked();

private:
    QSharedPointer<NdrApplication> ndrApp;
    QSharedPointer<PPPoE> pppoe;
    Ui::MainWindow *ui;
    LoginDialog *loginDialog;
    NoticeDialog *noticeDialog;
    SettingsDialog *settingsDialog;
    AboutDialog *aboutDialog;
    FeedbackDialog *feedbackDialog;
    QSystemTrayIcon *trayIcon;
    QMenu *myMenu;
    popUpDialog *popUp;

    //    QxtGlobalShortcut * logoffShortcut;

    State state;
    bool app_exiting; //退出标记
    bool isMainWindowMinimized;

    LocalStorage *profile;
    int connTime;
    int allTime;
    QString username;
    int timerId;

    // SettingsSet *settings;
    UpdateService *updateServer;
    //    InterfaceInfo *interfaceInfo;
    //    HttpsJsonPost *httpsJsonPost;
    void timerEvent(QTimerEvent *);
    void changeEvent(QEvent *event);

    QString time_humanable(int sec);

    // void sendData();

    void verifyEncrypt();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    //    void logoffShortcutActivated();
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void minimumWindow();
};

#endif // MAINWINDOW_H
