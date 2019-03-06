#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

namespace new_UI {
class QMLWindowsManager;
};

class QMLWindowsManager final : public QObject {
    Q_OBJECT
public:
    explicit QMLWindowsManager(QObject *parent = nullptr);

    ~QMLWindowsManager() = default;

    const QWindow *getWindow();

signals:

public slots:
    void btnLogin_clicked(const QString &username, const QString &passwd,
                          const QString &pack_info, const QString &NIC_info);

private:
    QSharedPointer<QQmlApplicationEngine> engineLoginDialog,
        engineMainWindow; // save qml engine
    QSharedPointer<QQmlComponent> compLoginDialog, compMainWindow;
    QSharedPointer<QWindow> loginDialog, mainWindow; // save window pointer

    QSharedPointer<QQmlApplicationEngine> engineManagerDialog;
    QSharedPointer<QQmlComponent> compManagerDialog;
    QSharedPointer<QWindow> managerWindows;

    void bind_slot();
    void dailFinish(bool status);
    void btnStopConnect_clicked();
    void bind_loginDialog_slot();
    void InitLoginDialog();
    void get_password(QString);
    void initMainWindow();
    void tryLogin(QString, QString, QString, QString, QString, QString);
    void timeEvent();
};
