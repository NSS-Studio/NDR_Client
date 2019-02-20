#include "qmlaboutdialog.hpp"
#include <QUrl>
#include <QQuickView>
#include <QQmlComponent>
#include <pppoe.hpp>
#include "utils.hpp"

QMLAboutDialog::QMLAboutDialog(QObject *parent) : QObject(parent) {
  engineLoginDialog = new QQmlApplicationEngine();
  compLoginDialog = new QQmlComponent(engineLoginDialog, QUrl("qrc:/qmlforms/loginDialog.qml"));
  loginDialog = static_cast<QWindow*>(compLoginDialog->create());
  engineMainWindow = new QQmlApplicationEngine();
  compMainWindow = new QQmlComponent(engineMainWindow, QUrl("qrc:/qmlforms/mainWindow.qml"));
  mainWindow = static_cast<QWindow*>(compMainWindow->create());
#ifndef QT_DEBUG
  mainWindow->hide();
#endif

  qDebug() << mainWindow;


  bind_loginDialog_slot();

  InitLoginDialog();
}

void QMLAboutDialog::btnLogin_clicked(const QString& username
                                      ,const QString& passwd
                                      ,const QString& pack_info
                                      ,const QString& NIC_info) {
    qDebug() << "username" << username;
    qDebug() << "pasword" << passwd;
    qDebug() << "pack_info" << pack_info;
    qDebug() << "nic_info" << NIC_info;

    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->dialRAS(NDR_PHONEBOOK_NAME,username,passwd,pack_info);

    mainWindow->show();
    initMainWindow();
    loginDialog->hide();
}

void QMLAboutDialog::bind_loginDialog_slot(){
    QObject::connect(loginDialog,SIGNAL(login(QString,QString,QString,QString)),this,SLOT(btnLogin_clicked(QString,QString,QString,QString)));
}

void QMLAboutDialog::InitLoginDialog() {
  QStringList postfitList = utils::getDrModelPostfixTable();

  int32_t arg2 = 0;
  for (auto const &postfit : postfitList) {
    qDebug() << postfit;
    QVariant arg1 = postfit;
    arg2 = arg2 + 1;
    QMetaObject::invokeMethod(loginDialog,
                              "addPost",
                              Qt::DirectConnection,
                              Q_ARG(QVariant, arg1),
                              Q_ARG(QVariant, QVariant(arg2))
                              );
  }
  QMetaObject::invokeMethod(loginDialog,
                            "getVersion",
                            Qt::DirectConnection,
                            Q_ARG(QVariant, QVariant(utils::getVersionString()))
                            );
  qDebug() << "add version successful";

#ifdef Q_OS_WIN
  QMetaObject::invokeMethod(loginDialog,
                            "def_windows",
                            Qt::DirectConnection
                            );
#endif
}

void QMLAboutDialog::initMainWindow () {
    QMetaObject::invokeMethod(mainWindow,
                              "getVersion",
                              Qt::DirectConnection,
                              Q_ARG(QVariant, QVariant(utils::getVersionString()))
                              );
}
