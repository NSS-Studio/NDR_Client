// singleApplication.h
#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H
#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
//#include "dlgmain.h"
#include <QtCore/QDebug>

class SingleApplication : public QApplication
{
	Q_OBJECT
public:
	explicit SingleApplication(int &argc, char* argv[], const QString &uniqueKey);
	bool isRunning();
	bool sendMessage(const QString & message);
	bool releaseSharedMemory();

public slots:
	void receiveMessage();
	void showMainWindow();

signals:
	void messageAvailable(QString message);

private:
	bool _isRunning;
	QString _uniqueKey;
	QSharedMemory sharedMemory;
	QLocalServer* localServer;
	static const int timeout = 1000;
};

#endif // SINGLEAPPLICATION_H
// singleApplication.cpp
