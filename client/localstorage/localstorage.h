#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QObject>
#include <QVector>
#include <QtSql>
#include <QSqlDatabase>
#include "settingsset.h"
class LocalStorage : public QObject
{
    Q_OBJECT
public:
    explicit LocalStorage(QString filename,QObject *parent = nullptr);
    ~LocalStorage();
    
    bool open();
    void close();
    
    QVector<QString> getUserNameList();
    
    void setLoginInfo(QString username,QString password,QString manner);
    bool getLoginInfo(QString username,QString &password,QString &manner);
    
    void setAutoLoginUser(QString username);
    bool removeLoginInfo(QString username);
    bool getAutoLoginUser(QString &username);
    bool getLastLoginUser(QString &username);
    
    void setUserOnlineTime(QString username,int time);
    bool getUserOnlineTime(QString username,int &time);
    
    void setMainWindowRect(QString username,int x,int y, int width, int height);
    bool getMainWindowRect(QString username,int &x,int &y, int &width, int &height);

    void setDeviceName(const QString &);
    bool getDeviceName(QString &);

    bool readSettings(SettingsSet &settings);
    void writeSettings(SettingsSet settings);
    
private:
    QSqlDatabase *db = nullptr;
    QSqlQuery *query = nullptr;
    int sum = 0;
signals:
    
public slots:
    
};

#endif // LOCALSTORAGE_H
