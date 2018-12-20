#include "ndrapplication.hpp"
#include "utils.hpp"
#include <QMessageBox>
#include <QTimer>
#include <utility>
NdrApplication::NdrApplication(QString appName, int &argc, char **argv)
    : QApplication{argc, argv}, appName{std::move(appName)} {

  localServer.reset(new QTcpServer{});

  auto localListenningState =
      localServer->listen(QHostAddress::Any, LOCAL_SERVER_PORT);

  if (!localListenningState) {
    QMessageBox::information(
        nullptr, QObject::tr("提示"),
        QObject::tr("打开失败\n检测到已经有一个实例正在运行。"));
    qDebug()
        << QString("local Server bind port %0 failed").arg(LOCAL_SERVER_PORT);
    QTimer::singleShot(0, this, &NdrApplication::quit);

  } else {
    qDebug() << "Create localTcpServer successful";

    utils::initAppHome();     //初始化 应用程序本地目录
    utils::initTempDir();     //初始化 应用临时文件夹
    utils::initSettingsSet(); //初始化 配置
    utils::initLanguage();    //初始化 语言

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    utils::initFont(":/font/ztgj.ttf");
    //初始化 默认字体,Windows他妈的字体渲染像屎一样，用默认吧
#endif
#if 0 //样式没法看，有时间好好把图P一P，暂时闭了
        __initStyleSheet(&a,":/style/"  "default"  ".qss"); //初始化 样式表
#endif
    utils::resourceManager.InitResourceManager();
    qDebug() << "Init configure successful";
  }
}
