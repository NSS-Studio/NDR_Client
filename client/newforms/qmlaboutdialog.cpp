#include "qmlaboutdialog.hpp"
#include <QUrl>
#include <QQuickView>
#include <QQmlComponent>
#include "utils.hpp"

QMLAboutDialog::QMLAboutDialog(QObject *parent) : QObject(parent) {
  engine = new QQmlApplicationEngine();
  comp = new QQmlComponent(engine, QUrl("qrc:/qmlforms/loginDialog.qml"));

  root = static_cast<QWindow*>(comp->create());

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
}

void QMLAboutDialog::s(QString){
    qDebug() << "1";
}

void QMLAboutDialog::bind_loginDialog_slot(){
    QObject::connect(root,SIGNAL(login(QString,QString,QString,QString)),this,SLOT(btnLogin_clicked(QString,QString,QString,QString)));
//    QObject::connect(root,SIGNAL(testSignal(QString)),this,SLOT(btnLogin_clicked(QString)));
}

void QMLAboutDialog::InitLoginDialog() {
  QStringList postfitList = utils::getDrModelPostfixTable();

  int32_t arg2 = 0;
  for (auto const &postfit : postfitList) {
    qDebug() << postfit;
    QVariant arg1 = postfit;
    arg2 = arg2 + 1;
    QMetaObject::invokeMethod(root,
                              "addPost",
                              Qt::DirectConnection,
                              Q_ARG(QVariant, arg1),
                              Q_ARG(QVariant, QVariant(arg2))
                              );
  }
  QVariant msg = utils::getVersionString();
  QMetaObject::invokeMethod(root,
                            "getVersion",
                            Qt::DirectConnection,
                            Q_ARG(QVariant, msg)
                            );

#ifdef Q_OS_WIN
  QMetaObject::invokeMethod(root,
                            "def_windows",
                            Qt::DirectConnection
                            );
#endif
}

void QMLAboutDialog::initMainWindow () {
    QMetaObject::invokeMethod(root,
                              "getVersion",
                              Qt::DirectConnection
                              );
}
