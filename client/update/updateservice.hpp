#ifndef UPDATESERVICE_HPP
#define UPDATESERVICE_HPP

#include <QDebug>
#include <QDomDocument>
#include <QMessageBox>
#include <QObject>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QtNetwork>
//#include <QNetworkAccessManager>
//#define UPDATE_PACKAGE_FILENAME "ndr-setup"

#ifdef Q_OS_WIN
#include <windows.h>
#elif defined Q_OS_LINUX
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#elif defined Q_OS_MAC
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>
#else
#error OS is not supported
#endif

class UpdateService : public QObject {
  Q_OBJECT
public:
  explicit UpdateService(const QString &serverAddr,
                         const QString &tempDirectory,
                         QObject *parent = nullptr);
  void checkUpdate();
  void downloadLatestPackage();
  // bool tryInstall();
  bool openPackage(QString &errMsg);
  bool isFinished();
  QString packagePath();

private:
  QString tempDir;
  QString ipAddress;
  QString packageUrl;
  QString packageFilename;
  bool running;
  QNetworkAccessManager nam;

  // add ssl config
  QSslConfiguration sslConf;

  QNetworkRequest request;
  QNetworkReply *reply;
  bool isConnectUpdateServerFail;
  QDataStream *out;
  bool downloadToFile(QString urlStr, QString filename, QString &errorMsg);
#ifdef Q_OS_MAC
  // bool checkInstallDirectory();
  bool getCurrentInstallDirectory(char *);
#endif

signals:
  void checkFinished(bool error, int ver_major, int ver_minor, QString errMsg);
  void downloadFinished(bool error, QString errMsg);

public slots:

private slots:
  void readyRead();
  void checkOriginGet();
  void originGetFinished();
};

#endif // UPDATESERVICE_HPP
