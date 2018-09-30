#ifndef COMMON_H
#define COMMON_H
#include <ndrconfig.hpp>

#include "settingsset.h"
//#include "ndrapplication.hpp"
#include <QApplication>
#include <QFontDatabase>
#include <QTranslator>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QtGlobal>



extern QString appHome; //家目录
extern void __initAppHome();

extern QString tempDir; //临时文件夹
extern void __initTempDir();

/** 设置项*/
extern SettingsSet *settings;

extern QTranslator ndr_tr, qt_tr;

extern void __initSettingsSet();
extern void __initLanguage();
extern void __initFont(QString fontFile);
//extern void __initStyleSheet(NdrApplication* a, QString sheetFile);

extern QString networkInterfaceCardName;
#include <utils.hpp>
#endif // COMMON_H
