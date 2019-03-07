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
    void dailFnish(const bool &);
private slots:
    void start(QString, QString, QString, QString, QString, QString);
    void stopConnect();
};
