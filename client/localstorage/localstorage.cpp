#include "localstorage.hpp"

LocalStorage::LocalStorage(QString const &fileName, QObject *parent)
    : QObject(parent) {
  db.reset(new QSqlDatabase{QSqlDatabase::addDatabase("QSQLITE")});
  db->setDatabaseName(fileName);
}

bool LocalStorage::open() {
  qDebug() << "LocalStorage::open()" << sum;
  if (sum > 0) {
    sum++;
    return true;
  }
  if (!db->open())
    return false;
  QString sql_create_user_table = "CREATE TABLE IF NOT EXISTS 'user' ("
                                  "'username'	  VARCHAR(11) PRIMARY KEY,"
                                  "'password'	  TEXT,"
                                  "'manner'	  VARCHAR(9)," ///最高9字节
                                  "'wnd_left'   INTEGER,"
                                  "'wnd_top'    INTEGER,"
                                  "'wnd_width'  INTEGER,"
                                  "'wnd_height' INTEGER,"
                                  "'oltime'     INTEGER DEFAULT 0"
                                  ");";
  QString sql_create_config_table = "CREATE TABLE IF NOT EXISTS 'config' ("
                                    "'id'         INTEGER PRIMARY KEY,"
                                    "'autologin'  VARCHAR(11),"
                                    "'lastlogin'  VARCHAR(11),"
                                    "'autorun'    INTEGER DEFAULT 0,"
                                    "'device'     VARCHAR(8)"
                                    ");";
  QString sql_create_config_field =
      "INSERT OR IGNORE INTO config (id, autologin) VALUES (0, NULL);";
  query = QSharedPointer<QSqlQuery>{new QSqlQuery(*db)};
  if (!query->exec(sql_create_user_table) ||
      (!query->exec(sql_create_config_table)) ||
      (!query->exec(sql_create_config_field))) {
    qDebug() << query->lastError().text();
    db->close();
    query.reset();
    return false;
  }
  sum = 1;
  return true;
}

void LocalStorage::close() {
  qDebug() << "LocalStorage::close()" << sum;
  if (sum > 1) {
    sum--;
    return;
  }
  query.reset();
  db->close();
  sum = 0;
}

QVector<QString> LocalStorage::getUserNameList() {
  QString sql = "SELECT username FROM user";
  if (!query->exec(sql)) {
    return QVector<QString>();
  } else {

    QVector<QString> list;
    while (query->next()) {
      list.append(query->value(0).toString());
    }
    return list;
  }
}

void LocalStorage::setLoginInfo(QString username, QString password,
                                QString manner) {
  QString sql = "select username from user where username=?;";
  query->clear();
  query->prepare(sql);
  query->bindValue(0, username);

  if (!query->exec()) {
    return;
  }
  if (query->next()) {

    sql = "UPDATE user SET password=?,manner=? WHERE username=?;";
    query->prepare(sql);
    query->bindValue(0, password);
    query->bindValue(1, manner);
    query->bindValue(2, username);
    if (!query->exec()) {
      return;
    }
  } else {

    // query = new QSqlQuery();
    // query->finish();
    sql = "INSERT INTO user (username,password,manner) VALUES(?,?,?)";
    query->prepare(sql);
    query->bindValue(0, username);
    query->bindValue(1, password);
    query->bindValue(2, manner);
    if (!query->exec()) {
      return;
    }
  }
  sql = "UPDATE config SET lastlogin=? WHERE id=0";
  query->prepare(sql);
  query->bindValue(0, username);
  if (!query->exec()) {
    qDebug() << query->lastError().text();
  }
}

bool LocalStorage::getLoginInfo(QString username, QString &password,
                                QString &manner) {
  QString sql = "SELECT password,manner FROM user WHERE username=?";
  query->prepare(sql);
  query->bindValue(0, username);
  if (!query->exec()) {
    QTextStream(stdout) << query->lastError().text() << endl;
    return false;
  }
  if (!query->next())
    return false;
  password = query->value(0).toString();
  manner = query->value(1).toString();
  return true;
}

bool LocalStorage::removeLoginInfo(QString username) {
  QString sql = "DELETE FROM user WHERE username=?";
  query->prepare(sql);
  query->bindValue(0, username);
  if (!query->exec()) {
    QTextStream(stdout) << query->lastError().text() << endl;
    return false;
  }
  return true;
}

void LocalStorage::setAutoLoginUser(QString username) {
  QString sql = "UPDATE config SET autologin=? WHERE id=0";
  query->prepare(sql);
  query->bindValue(0, username);
  if (!query->exec()) {
    QTextStream(stderr) << query->lastError().text() << endl;
  }
}

bool LocalStorage::getAutoLoginUser(QString &username) {
  QString sql{"SELECT autologin FROM config WHERE id=0"};
  if (!query->exec(sql)) {
    qDebug() << query->lastError().text() << endl;
    return false;
  }
  if (!query->next())
    return false;
  username = query->value(0).toString();
  return true;
}

void LocalStorage::setUserOnlineTime(QString username, int time) {
  QString sql{"UPDATE user SET oltime=? WHERE username=?"};
  query->prepare(sql);
  query->bindValue(0, time);
  query->bindValue(1, username);
  if (!query->exec()) {
    qDebug() << query->lastError().text() << endl;
  }
}

bool LocalStorage::getUserOnlineTime(QString username, int &time) {
  QString sql = "SELECT oltime FROM user WHERE username=?";
  query->prepare(sql);
  query->bindValue(0, username);
  if (!query->exec()) {
    qDebug() << query->lastError().text() << endl;
  }
  if (query->next()) {
    bool ok;
    time = query->value(0).toInt(&ok);
    if (!ok)
      time = 0;
    return true;
  }
  return false;
}

void LocalStorage::setMainWindowRect(QString username, int x, int y, int width,
                                     int height) {
  QString sql = "UPDATE user SET wnd_left=?,wnd_top=?,wnd_width=?,wnd_height=? "
                "WHERE username=?";
  query->prepare(sql);
  query->bindValue(0, x);
  query->bindValue(1, y);
  query->bindValue(2, width);
  query->bindValue(3, height);
  query->bindValue(4, username);
  if (!query->exec()) {
    qDebug() << query->lastError().text() << endl;
  }
}

bool LocalStorage::getMainWindowRect(QString username, int &x, int &y,
                                     int &width, int &height) {
  QString sql =
      "SELECT wnd_left,wnd_top,wnd_width,wnd_height FROM user WHERE username=?";
  query->prepare(sql);
  query->bindValue(0, username);
  if (!query->exec()) {
    qDebug() << query->lastError().text() << endl;
  }
  if (query->next()) {
    bool ok = true;
    x = query->value(0).toInt(&ok);
    if (!ok)
      return false;
    y = query->value(1).toInt(&ok);
    if (!ok)
      return false;
    width = query->value(2).toInt(&ok);
    if (!ok)
      return false;
    height = query->value(3).toInt(&ok);
    if (!ok)
      return false;
    return true;
  } else {
    return false;
  }
}
bool LocalStorage::getLastLoginUser(QString &username) {
  qDebug() << "Function: " << __PRETTY_FUNCTION__ << " enter";
  qDebug() << "query" << query;
  QString sql{"SELECT lastlogin FROM config WHERE id=0"};
  qDebug() << "step -1";

  if (!query->exec(sql)) {
    return false;
  }
  qDebug() << "step 0";
  if (!query->next())
    return false;
  qDebug() << "step 1";
  username = query->value(0).toString();
  return true;
}

void LocalStorage::setDeviceName(const QString &name) {
  qDebug() << "Function: " << __PRETTY_FUNCTION__ << " arg: " << name;
  QString sql("UPDATE config SET device=? WHERE id=0");
  query->prepare(sql);
  query->bindValue(0, name);
  if (!query->exec())
    qWarning() << query->lastError().text();
}

bool LocalStorage::getDeviceName(QString &name) {
  qDebug() << "Function: " << __PRETTY_FUNCTION__;
  QString sql("SELECT device FROM config WHERE id=0");
  if (!query->exec(sql) || !query->next()) {
    return false;
  }
  name = query->value(0).toString();
  return true;
}
