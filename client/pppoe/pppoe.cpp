#include "pppoe.h"

#include <QtCore/QDebug>

PPPoE::PPPoE(QObject *parent) :
    QThread(parent){
    basedsl = new BaseDsl(NDR_PHONEBOOK_NAME);
    stop_now=false;
    isRedial = false;
    connect(this,SIGNAL(finished()),this,SLOT(threadFinished()),Qt::QueuedConnection);
//#ifdef Q_OS_WIN
    //this->hRasConn = NULL;
//#endif
//#ifdef Q_OS_LINUX
//    qDBusRegisterMetaType<Connection>();
//#endif
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
    bool ret = basedsl->dial(username, password, device_name, errorMessage);
    if(!ret)
    {
        dialSucceed = false;
        if(isRedial)
            emit redialFinished(false);
        else
            emit dialFinished(false);
    }else
    {
        if(isRedial)
            emit redialFinished(true);
        else
            emit dialFinished(true);
        dialSucceed = true;
        for(;;)
        {
            if(stop_now)
            {
                //Log::write("disconnect naturally\n");
                //Log::write(QString::number(ret)+ " " + QString::number(stop_now)+"\n");
                basedsl->hangUp();
                //emit hangedUp(true);
                isDisconnectNaturally=true;
                break;
            }
            if(basedsl->isDisconnected())
            {
                qDebug() << "Network Disconnected";
                isDisconnectNaturally=false;
                //emit hangedUp(false);
                break;
            }
            this->sleep(1);
        }
    }
}

QString PPPoE::getIpAddress() {
    return basedsl->getIpAddress();
}

/*
bool PPPoE::waitForResult()
{
    this->wait();
    return hRasConn?true:false;
}
*/

QString PPPoE::getUserName()
{
    return this->username;
}



bool PPPoE::dialRAS(const QString &entryName, const QString &username, const QString &password, const QString &device_name)
{
	//qDebug() << "PPPoE::dialRAS" << "device_name" << device_name;
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
	return BaseDsl::get_available_interfaces();
}