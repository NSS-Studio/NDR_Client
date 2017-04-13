#ifndef INFOMODULE_H
#define INFOMODULE_H
#include <QFile>
#include <QProcess>
#include <QObject>
#include <QPushButton>
#include <QThread>



class infoModule: public QObject
{
    Q_OBJECT
public:
    explicit infoModule(QObject *parent = 0);

    void getInfo();
    ~infoModule();
private:
    QFile info;
    QString infoText;
    QProcess *myProcess;

    bool getOneInfo(QString const& program, QStringList const& arguments);

    bool getSystemInfo();
    bool getArpInfo();
    bool getIfconfigInfo();
    bool getNetstatInfo();
    bool getPingInfo();
    bool getNslookupInfo();

private slots:
    void writeInfo();
};

class InfoModuleThread: public QThread {
    Q_OBJECT
public:

    ~InfoModuleThread();
    void setButton(QPushButton* button);
    void run();
    static InfoModuleThread* getInstance();


private:
    explicit InfoModuleThread(QPushButton* button_, QObject* parent = nullptr);
    infoModule* info_;
    QPushButton* button_;
    static InfoModuleThread* instance;

};

#endif // INFOMODULE_H
