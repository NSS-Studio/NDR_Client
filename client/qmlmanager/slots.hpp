#pragma once
#include <QMessageBox>
#include <QObject>

namespace new_UI {
class Slots;
}
class Slots final : public QObject {
    Q_OBJECT
public:
    Slots() = default;
    ~Slots() = default;
    void dailFinish(const bool &);
public slots:
    void start(QString username, QString password, QString pack_info,
               QString NIC_info, QString rember, QString autologin);
#ifdef Q_OS_WIN32
    void resetWinsock();
#endif
private slots:
    void stopConnect(int);
    void changtAccount(QString account);
    void clearConfig();

private:
    QString username, password, devicename, manner;
    bool DeleteDirectory(const QString &);
    bool rember = false;
    bool autoLogin = false;
};
