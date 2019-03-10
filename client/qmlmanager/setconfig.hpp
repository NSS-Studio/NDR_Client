#pragma once
#include <QSharedPointer>
#include <QWindow>

namespace new_UI {
class readConfig;
};
class setConfig final : public QObject {
public:
    setConfig() = delete;
    static bool setUsername(QWindow *);
    static bool setPackInfo(QWindow *);
    static bool setDevice(QWindow *);
    static bool setTittle(QWindow *);
    static bool setLastLogin(QWindow *);
    static bool setLoginInfo(QWindow *, QString = nullptr, bool status = false);
};
