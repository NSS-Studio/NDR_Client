#ifndef NDRAPPLICATION_HPP
#define NDRAPPLICATION_HPP

#include <QApplication>
#include <QSharedMemory>
#include <QException>
#include <QTcpServer>
#include <QMessageBox>
#include "common.hpp"
#include "tcpserverexception.hpp"

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
