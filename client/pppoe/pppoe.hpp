#ifndef PPPOE_HPP
#define PPPOE_HPP
#include <QtCore>
#include <basedsl.hpp>
class PPPoE : public QThread {
	Q_OBJECT
public:
    explicit PPPoE(QObject *parent = nullptr);
	~PPPoE();
	void run();
	void kill();
	bool dialRAS(const QString &entryName, const QString &username, const QString &password, const QString &device_name);
	bool redialRAS();
	void hangUp();
    bool isDisconnect();

	QString lastError();
	QString getUserName();
	QString getIpAddress();

    QStringList getAvailableInterfaces();

private:
    QSharedPointer<BaseDsl> basedsl;
	QString entryName;
	QString username;
	QString password;
	QString device_name;
	QString errorMessage;
	bool preparePhoneBook();
    bool stop_now = false;
    bool isRedial = false;
	bool isDisconnectNaturally;
	bool dialSucceed;

signals:
	void dialFinished(bool);
	void redialFinished(bool);
	void hangedUp(bool);//正常退出？
private slots:
	void threadFinished();

};



#endif // PPPOE_HPP
