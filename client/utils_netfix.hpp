#pragma once

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <tchar.h>
#include <windows.h>
#include <winreg.h>

#define WinsockPATH "System\\CurrentControlSet\\Services\\Winsock"
#define Winsock2PATH "System\\CurrentControlSet\\Services\\Winsock2"

namespace utils {
class utils_netfix : public QObject {
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
} // namespace utils

