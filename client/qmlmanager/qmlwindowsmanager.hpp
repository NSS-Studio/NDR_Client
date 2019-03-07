#pragma once

#include "slots.hpp"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

namespace new_UI {
class QMLWindowsManager;
};

class QMLWindowsManager final : public QObject {
public:
    explicit QMLWindowsManager(QObject *parent = nullptr);

    ~QMLWindowsManager() = default;

private:
    QSharedPointer<QQmlApplicationEngine> engineManagerDialog;
    QSharedPointer<QQmlComponent> compManagerDialog;
    QSharedPointer<QWindow> managerWindows;
    QSharedPointer<Slots> slot;
    void bind_slot();
};
