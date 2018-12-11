#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "aboutdialog.hpp"
#include "feedbackdialog.hpp"
#include "getinfoaboutnss.hpp"
#include "hangupdialog.hpp"
#include "localstorage.hpp"
#include "logindialog.hpp"
#include "noticedialog.hpp"
#include "popupinfomationdialog.hpp"
#include "pppoe.hpp"
#include "resourcemanager.hpp"
#include "settingsdialog.hpp"
#include "updateservice.hpp"
#include <QSharedPointer>
namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  enum class State { Logining, Working, Others };

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
  void checkFinished(bool error, int major, int minor, QString const &errMsg);
  void downloadFinished(bool error, QString const &errMsg);
  void loginWindowClosed();
  void getMessageFromNSS();

  void on_goDnuiBrowser_clicked();

private:
  QSharedPointer<QSystemTrayIcon> trayIcon;
  QSharedPointer<Ui::MainWindow> ui;
  QSharedPointer<NoticeDialog> noticeDialog;
  QSharedPointer<SettingsDialog> settingsDialog;
  QSharedPointer<FeedbackDialog> feedbackDialog;
  QMenu *myMenu{};

  State state{};
  bool app_exiting; //退出标记
  bool isMainWindowMinimized;
  int connTime{};
  int allTime{};
  QString username;
  int timerId{};

  QSharedPointer<UpdateService> updateServer;
  void timerEvent(QTimerEvent *);
  void changeEvent(QEvent *event);

  QString time_humanable(int sec);

  void verifyEncrypt();

public slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
  void closeEvent(QCloseEvent *event);
signals:
  void minimumWindow();
};

#endif // MAINWINDOW_HPP
