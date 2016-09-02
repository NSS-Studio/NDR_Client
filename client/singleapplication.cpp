#include "singleapplication.h"

/*
 * CONSTRUCTION: set SingleApplication's shareMemory key to <uniqueKey>
 * 				test attach , if success, already has an instance running
 * 				if not running, use shareMemory to create a segment in the memory
 * 				new a server to listem for <uniqueKey>, once received, hand over to SLOT(receiveMessage())
 *
 */
SingleApplication::SingleApplication(int &argc, char *argv[], const QString &uniqueKey) :
    QApplication(argc, argv), _uniqueKey(uniqueKey)
{
    sharedMemory.setKey(_uniqueKey);
    if(sharedMemory.attach()){
        _isRunning = true;
    }
    else{
        _isRunning = false;
        if(!sharedMemory.create(1)){
            qDebug()<<"Unable to create single instance";
            return;
        }
        // create local server and listener to incomming message from other instances.
        localServer = new QLocalServer(this);
//        connect(localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
        connect(localServer, SIGNAL(newConnection()), this, SLOT(showMainWindow()));
        localServer->listen(_uniqueKey);
    }
}
// public slot
/*
 * RECIVER: waiting for connection.	once get one connection, create a socket to communicate with the client.
 *
 */
void SingleApplication::receiveMessage(){
    qDebug()<<"receiveMessage";
    QLocalSocket* localSocket = localServer->nextPendingConnection();
    if(!localSocket->waitForReadyRead(timeout)){
        qDebug()<<"localSocket waitForReadyRead failed.";
        qDebug()<<localSocket->errorString().toLatin1();
        return;
    }
    QByteArray byteArray = localSocket->readAll();
    QString message = QString::fromUtf8(byteArray.constData());
    emit messageAvailable(message);
    localSocket->disconnectFromServer();
}
void SingleApplication::showMainWindow(){
    QLocalSocket *localSocket = localServer->nextPendingConnection();
    if (!localSocket)
        return;
    delete localSocket;
    //DlgMain::shareInstance()->show();
}
// public functions;
bool SingleApplication::isRunning(){
    qDebug()<<"SingleApplication is running? "<<_isRunning;
    return _isRunning;
}
/*
 * SENDER: if there is already a instance running, create a Socket to communicate with It's Server.
 *
 */
bool SingleApplication::sendMessage(const QString &/*message*/){
    qDebug()<<"sendMessage";
    if(!_isRunning){
        return false;
    } // no instance, do nothing.
    QLocalSocket localSocket(this);
    localSocket.connectToServer(_uniqueKey, QIODevice::WriteOnly);
    if(!localSocket.waitForConnected(timeout)){
        qDebug()<<"waitForConnected";
        qDebug()<<localSocket.errorString().toLatin1();
        return false;
    }
    if(!localSocket.waitForBytesWritten(timeout)){
        qDebug()<<"waitForBytesWritten";
        qDebug()<<localSocket.errorString().toLatin1();
        return false;
    }
    localSocket.disconnectFromServer();
    return true;
}

bool SingleApplication::releaseSharedMemory() {
	return sharedMemory.detach();
}
