#pragma once

#include <QObject>
#include <QQuickItem>
#include <QQuickView>
#include <QQmlApplicationEngine>

class QMLAboutDialog : public QObject {
  Q_OBJECT
public:

  explicit QMLAboutDialog(QObject *parent = nullptr);

  ~QMLAboutDialog(){
      delete engineLoginDialog;
      delete engineMainWindow;
      delete compLoginDialog;
      delete compMainWindow;
      delete loginDialog;
      delete mainWindow;
  }

signals:

public slots:
    void btnLogin_clicked(const QString& username
                     ,const QString& passwd
                     ,const QString& pack_info
                     ,const QString& NIC_info);


private:
  QQmlApplicationEngine *engineLoginDialog, *engineMainWindow; // save qml engine
  QQmlComponent *compLoginDialog, *compMainWindow;
  QWindow *loginDialog, *mainWindow; // save window pointer

  void bind_slot();
  void dailFinish(bool status);
  void btnStopConnect_clicked();
  void bind_loginDialog_slot();
  void InitLoginDialog();
  void initMainWindow();
  void tryLogin(QString,QString,QString,QString,QString,QString);
  void timeEvent();
};
