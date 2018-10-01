#ifndef NDRAPPLICATION_HPP
#define NDRAPPLICATION_HPP

#include <QApplication>
#include <QTcpServer>
#include "tcpserverexception.hpp"
#include <pppoe.hpp>
class NdrApplication: public QApplication
{
public:
    NdrApplication(QString const& appName, int &argc, char **argv);
    ~NdrApplication();
private:
    QScopedPointer<QTcpServer> localServer;
    QString appName;
    bool isRunning;
};

#endif // NDRAPPLICATION_HPP
