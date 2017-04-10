#include "common.h"
#include "mainwindow.h"
#include "logindialog.h"
#include "pppoe.h"
#include "noticedialog.h"
#include "authenticat.h"
#include "singleapplication.h"
#include "feedbackdialog.h"
#include "hangupdialog.h"
#include <QtGui/QApplication>
#include <QSharedMemory>
#include <QObject>
#include <QtCore/QTextCodec>


int main(int argc, char *argv[])
{
#ifndef Q_OS_MAC
	QApplication::setStyle("cleanlooks");
#endif
	SingleApplication a(argc, argv,"NSS-DR-CLIENT");

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//设置源码文件字符串编码
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//设置源码中待翻译字符串编码

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
    w.hide();

	return a.exec();
}

