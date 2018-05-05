#ifndef COMMON_H
#define COMMON_H

#include "settingsset.h"
#include "singleapplication.h"
#include <QApplication>
#include <QFontDatabase>
#include <QTranslator>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QtGlobal>

#define autoStartRegRoot HKEY_CURRENT_USER
#define autoSatrtKeyWay "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define autoStartkeyName "NDR_AutoStart"

//.130 is down now
#ifndef NDR_TESTING
#define NDR_TESTING false
#endif

#define UNUSED(x) (void)(x)

/** server address*/
#define NDR_SERVER_INNER "172.24.5.130"
#define NDR_SERVER_OUTER "172.24.10.13"

#if (NDR_TESTING == true)
#define NDR_SERVER NDR_SERVER_INNER
#else
#define NDR_SERVER NDR_SERVER_OUTER
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 9, 3))
#error "!!! please use qt 5.9.3 or later"
#endif

/** 确定操作系统类型
 * 对APP_OS_STR 赋值 windows/linux/macos
 */
#if defined(Q_OS_WIN)
#define APP_OS_STR "windows"
#elif defined(Q_OS_LINUX)
#define APP_OS_STR "linux"
#elif defined(Q_OS_MAC)
#define APP_OS_STR "macos"
#else
#error OS is not supported
#endif

/** 确定处理器架构
 * x86/amd64
 */
#if defined(__i386__)
#define APP_ARCH_STR "i386"
#elif defined(__x86_64__)
#define APP_ARCH_STR "amd64"
#else
#error Arch is not supported
#endif

/** 编译时间*/
#define COMPL_DATATIME __DATE__ " " __TIME__
extern QString appHome; //家目录
extern void __initAppHome();

extern QString tempDir; //临时文件夹
extern void __initTempDir();
/** 语言存放文件夹*/
#define LANGUAGE_DIR_NAME "lang"

/** 网卡适配器名称*/
#define NDR_PHONEBOOK_NAME "NDR"

/** DRCOM服务器地址*/
#define DRCOM_SERVER_IP "172.24.253.35"

/** DRCOM服务器端口号*/
#define DRCOM_SERVER_PORT 61440

/** NDR主版本号*/
#define VERSION_MAJOR 0

/** NDR次版本号*/
#define VERSION_MINOR 76

/** NDR用户反馈服务器地址*/
#define NDR_FEEDBACK_SERVER "172.24.10.13"

/** NDR更新服务器地址和备用服务器地址*/
#define NDR_UPDATE_SERVER NDR_SERVER

/** NDR导航页*/
#define NDR_GATE "http://go.neusoft.edu.cn"

/** NDR弹浏览器网页*/
#define NDR_NSS NDR_SERVER

/** NDR通知弹窗地址*/
#define NDR_POPUP_URL                                                          \
    QString("https://%1/messages/popUp.xml").arg(QString(NDR_SERVER))

/** NDR主页*/
#define NDR_HOMESITE "https://ndr.neusoft.edu.cn"

/** 开启更新*/
#define ENABLE_UPDATE 1
extern QString __getVersionString();

/*
extern bool __getDrModelName(int id,QString &name);
extern bool __getDrModelPostfix(int id,QString &postfix);
extern QVector<int> __getDrModelIdList();
*/
/** 获取账户名套餐后缀列表*/
extern QStringList __getDrModelPostfixTable();

/** 根据套餐后缀获取套餐描述*/
extern bool __getDrModelCaption(QString postfix, QString &caption);

/** 获取语言所在路径，失败返回空字符串*/
extern QString getLangDir();

/** 获取语言文件列表*/
extern QStringList __getLangFileNameTable();

/**
 * @brief __getLanguageName 根据文件名获取语言名称
 * @param fileName 文件名
 * @param langName 待返回的语言名称字符串
 * @return 成功/失败
 */
extern bool __getLanguageName(QString fileName, QString &langName);

/** 设置项*/
extern SettingsSet *settings;

extern QTranslator ndr_tr, qt_tr;

extern void __initSettingsSet();
extern void __initLanguage();
extern void __initFont(QString fontFile);
extern void __initStyleSheet(SingleApplication *a, QString sheetFile);

extern QString networkInterfaceCardName;
#endif // COMMON_H
