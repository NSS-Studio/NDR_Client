#include "pppoe.hpp"
#include <utils.hpp>
#include "basedslfactory.hpp"
#include <QNetworkInterface>
PPPoE::PPPoE(QObject *parent) : QThread{parent}
{
    basedsl = BaseDslFactory::getCurrentPlatformBaseDsl();
    connect(this, &PPPoE::finished, this, &PPPoE::threadFinished, Qt::QueuedConnection);
}

QString PPPoE::lastError()
{
    return this->errorMessage;
}

PPPoE::~PPPoE()
{
    
}


void PPPoE::run() {
	//qDebug() << "PPPoE::run: device_name:" << device_name;
    //reconnect failed at here; windows bug?? when reconnect three times it will success
    //bool ret;
    //if (isRedial)
    //    for (int i = 0; i < 3 && !ret; i++)
    //        ret = basedsl -> dial(username, password, device_name, errorMessage);
    //else
    //    ret = basedsl -> dial(username, password, device_name, errorMessage);

    bool ret = basedsl->dial(username, password, device_name, errorMessage);
    qDebug() << "failed: " << errorMessage;
    if(ret){
        if(isRedial)
            emit redialFinished(true);
        else
            emit dialFinished(true);
        dialSucceed = true;
        while (1){
            if(stop_now){
                //Log::write("disconnect naturally\n");
                //Log::write(QString::number(ret)+ " " + QString::number(stop_now)+"\n");
                basedsl->hangUp();
                //emit hangedUp(true);
                isDisconnectNaturally=true;
                break;
            }
            if(basedsl->isDisconnected()){
                qDebug() << "Network Disconnected";
                isDisconnectNaturally=false;
                //emit hangedUp(false);
                break;
            }
            this->sleep(1);
        }
    } else {
        dialSucceed = false;
        if(isRedial)
            emit redialFinished(false);
        else
            emit dialFinished(false);

    }
    qDebug() << "break from pppoe";
}

QString PPPoE::getIpAddress() {
    auto interfaceList = QNetworkInterface::allInterfaces();

    for (auto const &interface: interfaceList) {
        if (interface.type() == QNetworkInterface::InterfaceType::Ppp) {
            qDebug() << interface.name();
            qDebug() << interface.type();
            qDebug() << interface.hardwareAddress();
            auto addressEntries = interface.addressEntries();
            qDebug() << addressEntries.size();
            auto address = addressEntries.front();
            qDebug() << address.ip();
            return address.ip().toString();
        }
    }
    return {};
    //return basedsl->getIpAddress();
}

QVariant PPPoE::getHostMacAddress() {
//    auto interfaceList = QNetworkInterface::allInterfaces();
//    for (auto const& interface : interfaceList) {
//        if (interface.type() == QNetworkInterface::InterfaceType::Ppp) {
//            return QVariant{interface.hardwareAddress()};
//        }
//    }
//    return QVariant{};
//#if defined (Q_OS_MACX) || defined (Q_OS_LINUX)

    qDebug() << "device_name: " << this->device_name;
    auto interface = QNetworkInterface::interfaceFromName(this->device_name);
    qDebug() << "device_mac: " << interface.hardwareAddress();
    return QVariant{interface.hardwareAddress()};
//#else
//    return {};
//#endif
}

QString PPPoE::getUserName()
{
    return this->username;
}



bool PPPoE::dialRAS(const QString &entryName, const QString &username, const QString &password, const QString &device_name)
{
    qDebug() << "PPPoE::dialRAS" << "device_name" << device_name;
    if(this->isRunning())
        return false;
    this->entryName = entryName;
    this->username = username;
    this->password = password;
    this->device_name = device_name;
    this->isRedial = false;
    this->stop_now = false;
    qDebug() << "username" << username;
    qDebug() << "password" << password;
    this->start();
    return true;
}
bool PPPoE::redialRAS()
{
    if(this->isRunning())
        return false;
    isRedial = true;
    this->stop_now = false;
    //this->dialRAS(this->entryName,this->username,this->password);
    this->start();
    return true;
}

void PPPoE::hangUp()
{
    stop_now = true;
}

bool PPPoE::preparePhoneBook()
{
    return false;
}

void PPPoE::threadFinished()
{
    if(dialSucceed)
    {
        dialSucceed = false;
        emit hangedUp(isDisconnectNaturally);
    }
}

QStringList PPPoE::getAvailableInterfaces() {
    return basedsl->getAvailableInterfaces();
}

bool PPPoE::isDisconnect()
{
    return basedsl->isDisconnected();
}
