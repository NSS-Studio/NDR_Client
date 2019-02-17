#include "qmlaboutdialog.hpp"
#include <QUrl>
#include <QQuickView>
#include <QQmlComponent>
#include "utils.hpp"

QMLAboutDialog::QMLAboutDialog(QObject *parent) : QObject(parent) {
  engine = new QQmlApplicationEngine();
  comp = new QQmlComponent(engine, QUrl("qrc:/qmlforms/mainWIndows.qml"));

  root = static_cast<QWindow*>(comp->create());

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

