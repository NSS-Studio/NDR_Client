#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include "common.h"
#include <QObject>
#include <QDialog>
#include <QVector>
#include <QSet>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QTextBrowser>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslConfiguration>
#include <QUrl>
#include <QDomDocument>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winuser.h>
#include <psapi.h>
#include <winbase.h>
/*
 * useful function:
 * GetForegroundWindow
 * GetWindowRect
 * 
 * need to use this struct ↓↓↓↓
 * windowinfo
 * 
 * useful enum:
 * WS_EX_TOPMOST, WS_CAPTION
 * 
*/
#endif

class popUpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit popUpDialog(QWidget *parent = nullptr);
    ~popUpDialog();

    void getXmlFromNSS(const QString &url);

private slots:
    void getXmlDone();
    void goNextPage();
    void goPrePage();
    void timeCount();

private:
    QDialog *popUp;
    QLabel *laTitle;
    QLabel *laAuth;
    QLabel *page;
    QLabel *closeTime;
    QLineEdit *liAuth;
    QLineEdit *liTitle;
    QTextBrowser *teText;
    QPushButton *nextPage;
    QPushButton *prePage;
    QTimer *time;
    int timePass;

    QHBoxLayout *hbAuth;
    QHBoxLayout *hbTitle;
    QHBoxLayout *hbPage;

    QVBoxLayout *mainLay;

    QSslConfiguration sslConf;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QNetworkRequest request;
    QDesktopWidget desktop;
    QSettings *history;

    struct message{
        bool isShow;
        QString title;
        QString auth;
        QString text;
    };
    QVector<message> group;
    QSet<QString> text;
    int historyNum;
    int pageNow;

    void showMessage();
    void resetTime();

#ifdef Q_OS_WIN
    bool checkWindow();
#endif
};

#endif // POPUPDIALOG_H
