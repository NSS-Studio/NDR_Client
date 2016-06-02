#ifndef PPPOE_H
#define PPPOE_H
#include <QtCore>
#include "log.h"
#include "basedsl.h"

class PPPoE : public QThread{
	Q_OBJECT
public:
	explicit PPPoE(QObject *parent=0);
	~PPPoE();
	void run();
	void kill();
	bool dialRAS(const QString &entryName, const QString &username, const QString &password, const QString &device_name);
	bool redialRAS();
	void hangUp();

	QString lastError();
	QString getUserName();
	QString getIpAddress();

	static QStringList getAvailableInterfaces();

private:
	BaseDsl *basedsl;
	QString entryName;
	QString username;
	QString password;
	QString device_name;
	QString errorMessage;
	bool preparePhoneBook();
	bool stop_now;
	bool isRedial;
	bool isDisconnectNaturally;
	bool dialSucceed;

signals:
	void dialFinished(bool);
	void redialFinished(bool);
	void hangedUp(bool);//正常退出？
private slots:
	void threadFinished();

};



#endif // PPPOE_H
