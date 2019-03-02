#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickView>

class QMLWindowsManager final : public QObject {
    Q_OBJECT
public:
    explicit QMLWindowsManager(QObject *parent = nullptr);

    ~QMLWindowsManager() = default;

signals:

public slots:
    void btnLogin_clicked(const QString &username, const QString &passwd,
                          const QString &pack_info, const QString &NIC_info);

private:
    QSharedPointer<QQmlApplicationEngine> engineLoginDialog,
        engineMainWindow; // save qml engine
    QSharedPointer<QQmlComponent> compLoginDialog, compMainWindow;
    QSharedPointer<QWindow> loginDialog, mainWindow; // save window pointer

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
