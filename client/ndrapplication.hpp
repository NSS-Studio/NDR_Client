#ifndef NDRAPPLICATION_HPP
#define NDRAPPLICATION_HPP

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

#endif // NDRAPPLICATION_HPP
