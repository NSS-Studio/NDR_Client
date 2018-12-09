#ifndef NDRAPPLICATION_HPP
#define NDRAPPLICATION_HPP

#include <QApplication>
#include <QTcpServer>
class ResourceManager;
class NdrApplication: public QApplication
{
public:
    NdrApplication(QString appName, int &argc, char **argv);
    ~NdrApplication();
private:
    QScopedPointer<QTcpServer> localServer;
    QString appName;
};

#endif // NDRAPPLICATION_HPP
