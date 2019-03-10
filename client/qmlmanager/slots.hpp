#pragma once
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
private slots:
    void stopConnect();
    void changtAccount(QString account);
    void clearConfig();

private:
    QString username, password, devicename, manner;
    bool DeleteDirectory(const QString &);
    bool rember = false;
    bool autoLogin = false;
};
