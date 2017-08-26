#include "common.h"
#include "mainwindow.h"
#include "logindialog.h"
#include "pppoe.h"
#include "noticedialog.h"
#include "authenticat.h"
#include "singleapplication.h"
#include "feedbackdialog.h"
#include "hangupdialog.h"
#include <QApplication>
#include <DbgHelp.h>
#include <QSharedMemory>
#include <QObject>
#include <QtCore/QTextCodec>

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){//程式异常捕获
    /*
      ***保存数据代码***
    */
    //创建 Dump 文件
    HANDLE hDumpFile = CreateFile(0 , GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QMessageBox::critical(NULL,"程式崩溃","<FONT size=4><div><b>对于发生的错误，表示诚挚的歉意</b><br/></div>"+
        QString("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
        QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}

int main(int argc, char *argv[])
{
#ifndef Q_OS_MAC
	QApplication::setStyle("cleanlooks");
#endif
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
	SingleApplication a(argc, argv,"NSS-DR-CLIENT");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//设置源码文件字符串编码
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//设置源码中待翻译字符串编码

    //a.setStyle("cde");

    a.setQuitOnLastWindowClosed(false);

    __initAppHome(); //初始化 应用程序本地目录
    __initTempDir(); //初始化 应用临时文件夹
    __initSettingsSet(); //初始化 配置
    __initLanguage(); //初始化 语言
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    __initFont(":/font/ztgj.ttf"); //初始化 默认字体,Windows他妈的字体渲染像屎一样，用 默认吧
#endif
#if 0 //样式没法看，有时间好好把图P一P，暂时闭了
    __initStyleSheet(&a,":/style/"  "default"  ".qss"); //初始化 样式表
#endif

    Log::setLogFilename(appHome + "/auth.log");
	//QSettings config(appHome + "/common.cfg", QSettings::IniFormat);
	//QString current_locale = config.value("Locale", QLocale::system().name()).toString();
	//qDebug() << "current_locale" << current_locale;

    // 防止进程多开
	if(a.isRunning())
	{
#ifdef Q_OS_UNIX
		QMessageBox::information(0,QObject::tr("提示"),QObject::tr("打开失败\n检测到已经有一个实例正在运行。如果客户端上一次异常退出，请重试。"));
#else
		QMessageBox::information(0,QObject::tr("提示"),QObject::tr("打开失败\n检测到已经有一个实例正在运行。"));
#endif
		return 0;
	}

    MainWindow w;
//    w.show();
    w.hide();

	return a.exec();
}

