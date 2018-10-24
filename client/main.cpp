#include <pppoe.hpp>
#include <ndrapplication.hpp>
#include <QApplication>
#include <QObject>
#include <QSharedMemory>
#include <QTextCodec>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>
#include <utils.hpp>
#include <QStyleFactory>
#include <qapplication.h>
#include <resourcemanager.hpp>

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlComponent>

#if defined(QT_DEBUG) && defined(Q_OS_WIN)
#include <DbgHelp.h>
#endif

#if defined(QT_DEBUG) && defined(Q_OS_WIN)
//! windows debug
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException) { //程式异常捕获
    /*
     ***保存数据代码***
     */
    //创建 Dump 文件
    HANDLE hDumpFile = CreateFile(0, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        // Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
                          MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16)),
        errAdr(QString::number((uint)record->ExceptionAddress, 16)), errMod;
    QMessageBox::critical(
        NULL, "程式崩溃",
        "<FONT size=4><div><b>对于发生的错误，表示诚挚的歉意</b><br/></div>" +
            QString("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>")
                .arg(errCode)
                .arg(errAdr),
        QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char *argv[]) {
    //  high dpi support!
    // this support is so bad, we can not use it
    qInstallMessageHandler(utils::messageHandler);
    QNetworkProxyFactory::setUseSystemConfiguration(false);

#if defined (Q_OS_MAC) || defined (Q_OS_WIN32)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#ifndef Q_OS_MAC
    QApplication::setStyle("cleanlooks");
#endif
#if defined(QT_DEBUG) && defined(Q_OS_WIN)
    SetUnhandledExceptionFilter(
        (LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    NdrApplication ndrApp("ndr-client-new", argc, argv);
    if (ndrApp.shouldExit()) {
        exit(-1);
    }

    ndrApp.setQuitOnLastWindowClosed(false);

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    utils::initFont(":/font/ztgj.ttf");
    //初始化 默认字体,Windows他妈的字体渲染像屎一样，用默认吧
#endif
#if 0 //样式没法看，有时间好好把图P一P，暂时闭了
    __initStyleSheet(&a,":/style/"  "default"  ".qss"); //初始化 样式表
#endif
//    utils::resourceManager.InitResourceManager();

//    QQmlEngine engine;
//    QQmlComponent component(&engine, QUrl("qrc:/qmlforms/PopUpInfomationForm.ui.qml"));
//    QObject *object = component.create();



//    QQuickView view;
////    view.setResizeMode(QQuickView::SizeRootObjectToView);
//    view.setSource(QUrl("qrc:/qmlforms/PopUpInfomationForm.ui.qml"));
//    auto text = view.rootObject()->findChild<QQuickItem*>("welcome");
//    if (text) {
//        text->setProperty("text", "哞菇， 咕咕咕");
//    } else {

//    }
//    view.setFlag(Qt::FramelessWindowHint, true);
//    view.show();

    return ndrApp.exec();
}
