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
    QString result = myProcess->readAllStandardOutput();

            //QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");
    QTextStream infoTextStream(&info);
    //QString result = utfCodec->toUnicode(myProcess.readAll());
    infoTextStream << result;
}

infoModule::~infoModule() {
    info.close();
    //delete info;
}

bool infoModule::getOneInfo(QString const& program, QStringList const& arguments) {
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
    delete myProcess;
    return true;
}

bool infoModule::getSystemInfo() {

    QString program;
    QStringList arguments;

#ifdef Q_OS_MAC
    program = "system_profiler";
#elif Q_OS_LINUX
    program = "cat"; // /proc/* >> text
    arguments << "/proc/*";
#elif Q_OS_WIN
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

InfoModuleThread::InfoModuleThread(QPushButton* button, QObject *parent)
    :QThread(parent), info_{new infoModule()}, button_{button}
{

}

InfoModuleThread::~InfoModuleThread()
{

}

void InfoModuleThread::run()
{
    button_->setEnabled(false);
    info_->getInfo();
    button_->setEnabled(true);
}

void InfoModuleThread::setButton(QPushButton* button) {
    button_ = button;
}

InfoModuleThread* InfoModuleThread::getInstance() {
    if (instance == nullptr) {
        instance = new InfoModuleThread(nullptr);
    }
    return instance;
}
