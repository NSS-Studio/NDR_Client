#include "ndrapplication.hpp"

NdrApplication::NdrApplication(QString const& appName, int &argc, char **argv)
    :QApplication{argc, argv},
      appName{appName},
      isRunning{false}
{

    localServer.reset(new QTcpServer{});

    auto localListenningState = localServer->listen(QHostAddress::Any, LOCAL_SERVER_PORT);

    if (localListenningState == false) {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("打开失败\n检测到已经有一个实例正在运行。"));
        throw TcpServerException{QString("local Server binds") + QString(LOCAL_SERVER_PORT) +" failed"};
    } else {
        this->isRunning = true;
    }

    qDebug() << "Create localTcpServer successful";
}

NdrApplication::~NdrApplication() {

}
