#ifndef POPUPDIALOG_HPP
#define POPUPDIALOG_HPP

#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QDomDocument>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPushButton>
#include <QSet>
#include <QSettings>
#include <QSharedPointer>
#include <QSslConfiguration>
#include <QString>
#include <QTextBrowser>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>
#include <QVector>

#ifdef Q_OS_WIN
#include <psapi.h>
#include <winbase.h>
#include <windows.h>
#include <winuser.h>
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

class PopUpInfomationDialog final : public QObject {
  Q_OBJECT
public:
  explicit PopUpInfomationDialog();
  ~PopUpInfomationDialog();
  void getXmlFromNSS(QString const &url);

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

  struct message {
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

#endif // POPUPDIALOG_HPP
