#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include "settingsset.hpp"
#include <QObject>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QVector>
#include <QtSql>
class LocalStorage final : public QObject {
  Q_OBJECT
public:
  explicit LocalStorage(QString const &fileName, QObject *parent = nullptr);
  ~LocalStorage() = default;

  bool open();
  void close();

  QVector<QString> getUserNameList();

  void setLoginInfo(QString username, QString password, QString manner);
  bool getLoginInfo(QString username, QString &password, QString &manner);

  void setAutoLoginUser(QString username);
  bool removeLoginInfo(QString username);
  bool getAutoLoginUser(QString &username);
  bool getLastLoginUser(QString &username);

  void setUserOnlineTime(QString username, int time);
  bool getUserOnlineTime(QString username, int &time);

  void setMainWindowRect(QString username, int x, int y, int width, int height);
  bool getMainWindowRect(QString username, int &x, int &y, int &width,
                         int &height);

  void setDeviceName(const QString &);
  bool getDeviceName(QString &);

  bool readSettings(SettingsSet &settings);
  void writeSettings(SettingsSet settings);

private:
  QSharedPointer<QSqlDatabase> db;
  QSharedPointer<QSqlQuery> query;
  int sum{};
};

#endif // LOCALSTORAGE_H
