#include "ndrapplication.hpp"
#include <utils.hpp>
#include <QMessageBox>
NdrApplication::NdrApplication(QString const& appName, int &argc, char **argv)
    :QApplication{argc, argv},
      appName{appName}
{

    localServer.reset(new QTcpServer{});

    auto localListenningState = localServer->listen(QHostAddress::Any, LOCAL_SERVER_PORT);

    if (localListenningState == false) {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("打开失败\n检测到已经有一个实例正在运行。"));
        qDebug() << QString("local Server bind port %0 failed").arg(LOCAL_SERVER_PORT);
        // Don't use qApp->exit()
        // Because qApp don't run function exec()
        // So qApp->exit() will do Nothing

        // quick_exit is c++ function
        // Don't use exit because quick_exit don't run deconstruct function
        quick_exit(EXIT_FAILURE);
    } else {
        qDebug() << "Create localTcpServer successful";

        utils::initAppHome();     //初始化 应用程序本地目录
        utils::initTempDir();     //初始化 应用临时文件夹
        utils::initSettingsSet(); //初始化 配置
        utils::initLanguage();    //初始化 语言
        qDebug() << "Init configure successful";
    }
}

NdrApplication::~NdrApplication() {

}
