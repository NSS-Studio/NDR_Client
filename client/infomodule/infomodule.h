#ifndef INFOMODULE_H
#define INFOMODULE_H
#include <QFile>
#include <QProcess>
#include <QObject>

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

#endif // INFOMODULE_H
