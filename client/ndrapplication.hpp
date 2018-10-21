#ifndef NDRAPPLICATION_HPP
#define NDRAPPLICATION_HPP

#include <QApplication>
#include <QTcpServer>
class NdrApplication: public QApplication
{
public:
    NdrApplication(QString const& appName, int &argc, char **argv);
    ~NdrApplication();
    bool shouldExit();
private:
    QScopedPointer<QTcpServer> localServer;
    QString appName;
    bool exitState = false;
    bool isRunning = false;
};

#endif // NDRAPPLICATION_HPP
