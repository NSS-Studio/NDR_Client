#include "infomodule.h"
#include <QProcess>
#include <QDesktopServices>
#include <QDebug>
#include <QTextCodec>

infoModule::infoModule(QObject *parent)
    :QObject(parent)
{
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    desktop += QString("/info.txt");
    infoText = desktop;
    info.setFileName(infoText);
    info.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
}

void infoModule::getInfo() {
    getSystemInfo();
    getArpInfo();
    getIfconfigInfo();
    getNetstatInfo();
    getPingInfo();
    getNslookupInfo();
}

void infoModule::writeInfo() {
    //QString result = myProcess->readAllStandardOutput();

#ifdef Q_OS_WIN
    #define localCode "gbk"
#endif
#ifdef Q_OS_LINUX
    #define localCode "utf8"
#endif
#ifdef Q_OS_MAC
    #define localCode "utf8"
#endif
    qDebug() << "call writeInfo: ";
    QTextCodec* utfCodec = QTextCodec::codecForName(localCode);
    QString result = utfCodec -> toUnicode(myProcess->readAllStandardOutput());
    if (!result.isEmpty()) {
        QTextStream infoTextStream(&info);
        //QString result = utfCodec->toUnicode(myProcess.readAll());
        infoTextStream.setCodec(localCode);
        infoTextStream << result;
    } else
        qDebug() <<"call writeInfo, result is empty!!";
}

infoModule::~infoModule() {
    info.close();
    //delete info;
}

bool infoModule::getOneInfo(QString const& program, QStringList const& arguments) {
    qDebug() << "call getOneInfo";
    myProcess = new QProcess();
    connect(myProcess, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(writeInfo()));
    myProcess->start(program, arguments);
    if (!myProcess->waitForStarted())
        return false;

    myProcess->closeWriteChannel();

    if (!myProcess->waitForFinished())
        return false;

    myProcess->disconnect(this);
    myProcess -> deleteLater();
    return true;

}

bool infoModule::getSystemInfo() {

    QString program;
    QStringList arguments;

#ifdef Q_OS_MAC
    program = "system_profiler";
#elif defined(Q_OS_LINUX)
    program = "cat"; // /proc/* >> text
    arguments << "/proc/*";
#elif defined(Q_OS_WIN)
    program = "systeminfo";
#endif
    return getOneInfo(program, arguments);
}

bool infoModule::getArpInfo() {
    QString program = "arp";
    QStringList arguments;
    arguments << "-an";
    return getOneInfo(program, arguments);
}

bool infoModule::getIfconfigInfo() {
    QString program;
    QStringList arguments;

#ifdef Q_OS_WIN
    program = "ipconfig";
#else
    program = "ifconfig";
#endif

#ifdef Q_OS_WIN
    arguments << "/all";
#else
    arguments << "-a";
#endif
    return getOneInfo(program, arguments);
}

bool infoModule::getNetstatInfo() {
    QString program = "netstat";
    QStringList arguments;
    arguments << "-an";
    return getOneInfo(program, arguments);
}

bool infoModule::getPingInfo() {
    QString program = "ping";
    QStringList arguments;
#ifdef Q_OS_WIN
    arguments << "-n";
#else
    arguments << "-a";
#endif
    arguments << "www.baidu.com";
    arguments << "newjw.neusoft.edu.cn";
    return getOneInfo(program, arguments);
}

bool infoModule::getNslookupInfo() {
    QString program = "nslookup";
    QStringList arguments1, arguments2;
    arguments1 << "www.baidu.com";
    arguments2 << "newjw.neusoft.edu.cn";
    return getOneInfo(program, arguments1)
            && getOneInfo(program, arguments2);
}

InfoModuleThread * InfoModuleThread::instance;

InfoModuleThread::InfoModuleThread(QObject *parent)
    :QThread(parent), info_{new infoModule()}
{

}

InfoModuleThread::~InfoModuleThread()
{

}

void InfoModuleThread::run()
{
//    button_->setEnabled(false);
//    info_->getInfo();
//    button_->setEnabled(true);
    qDebug() << "call thread run";
    info_->getInfo();
    emit infoGetFinished();
}

//void InfoModuleThread::setButton(QPushButton* button) {
//    button_ = button;
//}

void InfoModuleThread::startGetInfoToWriteFile() {
    //getInstance()->start();
    qDebug() << "call startGetInfoToWriteFile";
    this->start();
}


InfoModuleThread* InfoModuleThread::getInstance() {
    if (instance == nullptr) {
        instance = new InfoModuleThread(nullptr);
    }
    return instance;
}
