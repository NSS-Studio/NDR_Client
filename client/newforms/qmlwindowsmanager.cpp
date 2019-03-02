#include "qmlwindowsmanager.hpp"
#include "localstorage.hpp"
#include "pppoe.hpp"
#include "utils.hpp"
#include <QMessageBox>
#include <QQmlComponent>
#include <QQuickStyle>
#include <QQuickView>
#include <QUrl>
#include <pppoe.hpp>

QMLWindowsManager::QMLWindowsManager(QObject *parent) : QObject(parent) {
    QQuickStyle::setStyle("Universal");
    engineLoginDialog = QSharedPointer<QQmlApplicationEngine>::create();
    engineMainWindow = QSharedPointer<QQmlApplicationEngine>::create();
    compLoginDialog = QSharedPointer<QQmlComponent>::create(
        engineLoginDialog.get(),
        QUrl{QString{"qrc:/qmlforms/"} + QML_PLATFROM_GET(loginDialog.qml)});
    compMainWindow = QSharedPointer<QQmlComponent>::create(
        engineMainWindow.get(),
        QUrl{QString{"qrc:/qmlforms/"} + QML_PLATFROM_GET(mainWindow.qml)});
    loginDialog.reset(static_cast<QWindow *>(compLoginDialog->create()));
    mainWindow.reset(static_cast<QWindow *>(compMainWindow->create()));
#ifndef QT_DEBUG
    mainWindow->hide();
#endif
    qDebug() << mainWindow;

    bind_slot();

    InitLoginDialog();
}

void QMLWindowsManager::btnLogin_clicked(const QString &username,
                                         const QString &passwd,
                                         const QString &pack_info,
                                         const QString &NIC_info) {
    QString model = utils::getDrModel(pack_info);
    auto realUsername = "\r\n" + username + model;

    qDebug() << "username" << realUsername;
    qDebug() << "pasword" << passwd;
    qDebug() << "pack_info" << pack_info;
    qDebug() << "nic_info" << NIC_info;

    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->dialRAS(NDR_PHONEBOOK_NAME, realUsername, passwd, NIC_info);
    pppoe->start();

    //    mainWindow->show();
    //    initMainWindow();
    //    loginDialog->hide();
}

void QMLWindowsManager::bind_slot() {
    QObject::connect(mainWindow.get(), SIGNAL(stopConnect()), this,
                     SLOT(btnStopConnect_clicked()));
    QObject::connect(
        loginDialog.get(), SIGNAL(login(QString, QString, QString, QString)),
        this, SLOT(btnLogin_clicked(QString, QString, QString, QString)));
    QObject::connect(utils::resourceManager.getPPPoE(), &PPPoE::dialFinished,
                     this, &QMLWindowsManager::dailFinish);
    QObject::connect(loginDialog.get(), SIGNAL(change_account_select(QString)),
                     this, SLOT(get_password(QString)));
}

void QMLWindowsManager::get_password(QString account) {}

void QMLWindowsManager::dailFinish(bool status) {
    if (status == true) {
        this->loginDialog->hide();
        this->mainWindow->show();
        qDebug() << ("login success");
        loginDialog->findChild<QObject *>("panel1")->setProperty("visible",
                                                                 "true");
        loginDialog->findChild<QObject *>("panel3")->setProperty("visible",
                                                                 "false");
        loginDialog->findChild<QObject *>("loginButton")
            ->setProperty("visible", "true");
        initMainWindow();
        mainWindow->show();
    } else {
        qDebug() << ("login field");
        loginDialog->findChild<QObject *>("panel1")->setProperty("visible",
                                                                 "true");
        loginDialog->findChild<QObject *>("panel3")->setProperty("visible",
                                                                 "false");
        loginDialog->findChild<QObject *>("loginButton")
            ->setProperty("visible", "true");
    }
}

void QMLWindowsManager::btnStopConnect_clicked() {
    auto pppoe = utils::resourceManager.getPPPoE();
    pppoe->hangUp();
    loginDialog->show();
    mainWindow->findChild<QObject *>("startTime")
        ->setProperty("running", "false");
#ifndef QT_DEBUG
    mainWindow.hide()
#endif
}

void QMLWindowsManager::InitLoginDialog() {
    QStringList postfitList = utils::getDrModelPostfixTable();

    int32_t arg2 = 0;
    for (auto const &postfit : postfitList) {
        qDebug() << postfit;
        QVariant arg1 = postfit;
        arg2 = arg2 + 1;
        QMetaObject::invokeMethod(loginDialog.get(), "addPost",
                                  Qt::DirectConnection, Q_ARG(QVariant, arg1),
                                  Q_ARG(QVariant, QVariant(arg2)));
    }
    auto pppoe = utils::resourceManager.getPPPoE();
    QStringList interfaces = pppoe->getAvailableInterfaces();
    //  interface.clear(); // test info
    if (interfaces.count() == 1) {
        loginDialog->findChild<QObject *>("selectDevice")
            ->setProperty("visible", "false");
    }
#ifndef Q_OS_WIN32
    if (interfaces.count() == 0) {
        auto status = loginDialog->findChild<QObject *>("status");
        status->setProperty("color", "red");
        status->setProperty("text", "无可用网卡");
        auto login_btn = loginDialog->findChild<QObject *>("loginButton");
        login_btn->setProperty("visible", "false");
    } else {
        for (auto const &interface : interfaces) {
            QMetaObject::invokeMethod(loginDialog.get(), "addDevice",
                                      Qt::DirectConnection,
                                      Q_ARG(QVariant, QVariant(interface)));
        }
    }
#endif

    QMetaObject::invokeMethod(
        loginDialog.get(), "getVersion", Qt::DirectConnection,
        Q_ARG(QVariant, QVariant(utils::getVersionString())));
    qDebug() << "add version successful";

#ifdef Q_OS_WIN
    QMetaObject::invokeMethod(loginDialog.get(), "def_windows", Qt::DirectConnection);
#endif

    auto profile = utils::resourceManager.getProfile();
    if (profile->open()) {
        auto vec = profile->getUserNameList();
        for (const auto &tmp : vec) {
            qDebug() << tmp;
            QMetaObject::invokeMethod(loginDialog.get(), "addUsernameInfo",
                                      Qt::DirectConnection,
                                      Q_ARG(QVariant, QVariant{tmp}));
        }
        loginDialog->findChild<QObject *>("account")->setProperty(
            "currentIndex", 0);
    }
}

void QMLWindowsManager::initMainWindow() {
    QMetaObject::invokeMethod(
        mainWindow.get(), "getVersion", Qt::DirectConnection,
        Q_ARG(QVariant, QVariant(utils::getVersionString())));
    auto pppoe = utils::resourceManager.getPPPoE();

    QString base = pppoe->getUserName();

    QString username = base.mid(0, base.lastIndexOf("@"));
    QString pack_info = base.mid(base.lastIndexOf("@") + 1, base.size());

    this->mainWindow->findChild<QObject *>("userName")
        ->setProperty(
            "text",
            username.mid(username.lastIndexOf("\n") + 1,
                         username.count())); // add username on mainWindow
    auto model = utils::getDrModel(pack_info);
    auto modelCaption = utils::getDrModelCaption(model);
    if (!modelCaption.isValid())
        this->mainWindow->findChild<QObject *>("net")->setProperty(
            "text", pack_info); // add pack_info on mainWindow
    else
        this->mainWindow->findChild<QObject *>("net")->setProperty("text",
                                                                   "未知");

    mainWindow->findChild<QObject *>("ipText")->setProperty(
        "text", pppoe->getIpAddress());
    QMetaObject::invokeMethod(mainWindow.get(), "getDailTime",
                              Qt::DirectConnection);
    mainWindow->findChild<QObject *>("startTime")
        ->setProperty("running", "true");
}

void QMLWindowsManager::timeEvent() {}
