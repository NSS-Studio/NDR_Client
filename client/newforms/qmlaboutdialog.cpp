#include "qmlaboutdialog.hpp"
#include <QUrl>
#include <QQuickView>
#include <QQmlComponent>
#include <QMessageBox>
#include <pppoe.hpp>
#include "utils.hpp"
#include "pppoe/pppoe.hpp"

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
    QString model = utils::getDrModel(pack_info);
    auto realUsername = "\r\n" + username + model;

    qDebug() << "username" << realUsername;
    qDebug() << "pasword" << passwd;
    qDebug() << "pack_info" << pack_info;
    qDebug() << "nic_info" << NIC_info;

    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->dialRAS(NDR_PHONEBOOK_NAME,realUsername,passwd,NIC_info);

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
  auto pppoe = utils::resourceManager.getPPPoE();
#ifndef Q_OS_WIN32
  QStringList interfaces = pppoe->getAvailableInterfaces();
//  interface.clear(); // test info
  if (interfaces.count() == 1) {
      loginDialog->findChild<QObject*>("selectDevice")->setProperty("visible","false");
  }
  if (interfaces.count() == 0) {
    auto status = loginDialog->findChild<QObject*>("status");
    status->setProperty("color","red");
    status->setProperty("text","无可用网卡");
    auto login_btn = loginDialog->findChild<QObject*>("loginButton");
    login_btn->setProperty("visible","false");
  }
  else {
      for (auto interface: interfaces){
        QMetaObject::invokeMethod(
                                  loginDialog,
                                  "addDevice",
                                  Qt::DirectConnection,
                                  Q_ARG(QVariant,QVariant(interface))
                    );
      }
  }
#endif

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
    auto pppoe = utils::resourceManager.getPPPoE();

    QString base = pppoe->getUserName();

    QString username = base.mid(0,base.lastIndexOf("@"));
    QString pack_info = base.mid(base.lastIndexOf("@")+1,base.size());

    this->mainWindow->findChild<QObject*>("userName")->setProperty("text",username); // add username on mainWindow
    auto model = utils::getDrModel(pack_info);
    auto modelCaption = utils::getDrModelCaption(model);
    if (modelCaption.isValid())
      this->mainWindow->findChild<QObject*>("net")->setProperty("text",pack_info); // add pack_info on mainWindow
    else
      this->mainWindow->findChild<QObject*>("net")->setProperty("text","未知");

    mainWindow->findChild<QObject*>("ipText")->setProperty("text",pppoe->getIpAddress());
    QMetaObject::invokeMethod(mainWindow,
                              "getDailTime",
                              Qt::DirectConnection
                );
}

void QMLAboutDialog::timeEvent() {

}
