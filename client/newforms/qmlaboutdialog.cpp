#include "qmlaboutdialog.hpp"
#include <QUrl>
#include <QQuickView>
#include <QQmlComponent>
#include <QMessageBox>
#include <pppoe.hpp>
#include <QQuickStyle>
#include "utils.hpp"
#include "pppoe/pppoe.hpp"

QMLAboutDialog::QMLAboutDialog(QObject *parent) : QObject(parent) {
    LocalStorage* profile = utils::resourceManager.getProfile();
    qDebug () << profile;





  QQuickStyle::setStyle("Universal");
  engineLoginDialog = new QQmlApplicationEngine{};
  engineMainWindow = new QQmlApplicationEngine{};
#ifdef Q_OS_MAC
  compLoginDialog = new QQmlComponent{engineLoginDialog, QUrl{"qrc:/qmlforms/macLoginDialog.qml"}};
  compMainWindow = new QQmlComponent{engineMainWindow,QUrl{"qrc:/qmlforms/macMainWindow.qml"}};
#else
  compLoginDialog = new QQmlComponent(engineLoginDialog, QUrl("qrc:/qmlforms/loginDialog.qml"));
  compMainWindow = new QQmlComponent{engineMainWindow, QUrl{"qrc:/qmlforms/mainWindow.qml"}};
#endif
  loginDialog = static_cast<QWindow*>(compLoginDialog->create());
  mainWindow = static_cast<QWindow*>(compMainWindow->create());
#ifndef QT_DEBUG
  mainWindow->hide();
#endif

  qDebug() << mainWindow;


  bind_slot();

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
    pppoe->start();

//    mainWindow->show();
//    initMainWindow();
//    loginDialog->hide();
}

void QMLAboutDialog::bind_slot(){
    QObject::connect(mainWindow,SIGNAL(stopConnect()),this,SLOT(btnStopConnect_clicked()));
    QObject::connect(loginDialog,SIGNAL(login(QString,QString,QString,QString)),this,SLOT(btnLogin_clicked(QString,QString,QString,QString)));
    QObject::connect(utils::resourceManager.getPPPoE(),&PPPoE::dialFinished,this,&QMLAboutDialog::dailFinish);
}

void QMLAboutDialog::dailFinish(bool status) {
    if (status == true) {
        this->loginDialog->hide();
        this->mainWindow->show();
        qDebug () << ("login success");
        loginDialog->findChild<QObject*>("panel1")->setProperty("visible","true");
        loginDialog->findChild<QObject*>("panel3")->setProperty("visible","false");
        loginDialog->findChild<QObject*>("loginButton")->setProperty("visible","true");
        initMainWindow();
        mainWindow->show();
    }
    else {
        qDebug () << ("login field");
        loginDialog->findChild<QObject*>("panel1")->setProperty("visible","true");
        loginDialog->findChild<QObject*>("panel3")->setProperty("visible","false");
        loginDialog->findChild<QObject*>("loginButton")->setProperty("visible","true");
    }
}

void QMLAboutDialog::btnStopConnect_clicked() {
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->hangUp();
    loginDialog->show();
    mainWindow->findChild<QObject*>("startTime")->setProperty("running","false");
#ifndef QT_DEBUG
    mainWindow.hide()
#endif

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
  QStringList interfaces = pppoe->getAvailableInterfaces();
//  interface.clear(); // test info
  if (interfaces.count() == 1) {
      loginDialog->findChild<QObject*>("selectDevice")->setProperty("visible","false");
  }
#ifndef Q_OS_WIN32
  if (interfaces.count() == 0) {
    auto status = loginDialog->findChild<QObject*>("status");
    status->setProperty("color","red");
    status->setProperty("text","无可用网卡");
    auto login_btn = loginDialog->findChild<QObject*>("loginButton");
    login_btn->setProperty("visible","false");
  }
  else {
      for (auto const &interface: interfaces){
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

    this->mainWindow->findChild<QObject*>("userName")->setProperty("text",username.mid(username.lastIndexOf("\n")+1,username.count())); // add username on mainWindow
    auto model = utils::getDrModel(pack_info);
    auto modelCaption = utils::getDrModelCaption(model);
    if (!modelCaption.isValid())
      this->mainWindow->findChild<QObject*>("net")->setProperty("text",pack_info); // add pack_info on mainWindow
    else
      this->mainWindow->findChild<QObject*>("net")->setProperty("text","未知");

    mainWindow->findChild<QObject*>("ipText")->setProperty("text",pppoe->getIpAddress());
    QMetaObject::invokeMethod(mainWindow,
                              "getDailTime",
                              Qt::DirectConnection
                );
    mainWindow->findChild<QObject*>("startTime")->setProperty("running","true");
}

void QMLAboutDialog::timeEvent() {

}
