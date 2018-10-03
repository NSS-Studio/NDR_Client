#ifndef UTILS_NETFIX_H
#define UTILS_NETFIX_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <windows.h>
#include <winreg.h>
#include <tchar.h>

namespace utils {
#define WinsockPATH "System\\CurrentControlSet\\Services\\Winsock"
#define Winsock2PATH "System\\CurrentControlSet\\Services\\Winsock2"

class utils_netfix : public QObject
{
    Q_OBJECT
public:
    explicit utils_netfix(QObject *parent = nullptr);
    ~utils_netfix();

    void backUpRegistry(const QString &path);
    void fixNetWork();

 signals:
    void workFinish();

 private:
    void cmd(const QString &program, const QStringList &arg);
    void backUp(HKEY proxyRoot, const QString &keyWay, const QString &saveWay);
};

}

#endif // UTILS_NETFIX_H
