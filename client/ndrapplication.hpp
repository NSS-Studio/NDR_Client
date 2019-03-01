#pragma once

#include <QApplication>
#include <QTcpServer>
class ResourceManager;
class NdrApplication : public QApplication {
public:
  explicit NdrApplication(QString appName, int &argc, char **argv);
  ~NdrApplication() = default;

private:
  QScopedPointer<QTcpServer> localServer;
  QString appName;
};
