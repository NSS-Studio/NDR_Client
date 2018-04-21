#ifndef INFOMODULE_H
#define INFOMODULE_H
#include <QFile>
#include <QProcess>
#include <QObject>
#include <QPushButton>
#include <QThread>

#ifdef Q_OS_WIN
    #define localCode "gbk"
#elif defined (Q_OS_LINUX)
    #define localCode "utf8"
#elif defined (Q_OS_MAC)
    #define localCode "utf8"
#else
    #error "The platform isn't supported!"
#endif


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
    void run();
    static InfoModuleThread* getInstance();

signals:
    void infoGetFinished();

public slots:
    void startGetInfoToWriteFile();


private:
    explicit InfoModuleThread(QObject* parent = nullptr);
    infoModule* info_;
    static InfoModuleThread* instance;
};

#endif // INFOMODULE_H
