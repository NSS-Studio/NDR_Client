#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T08:30:29
# 客户端（包含UI接口）
#-------------------------------------------------

QT       += core gui network sql xml

TARGET = ndr-client #目标文件
TEMPLATE = app

#CONFIG += console #终端可作为调试输出载体
#DEFINES += QT_NO_DEBUG_OUTPUT #调试输入开关
include(authenticat/authenticat.pri)
include(pppoe/pppoe.pri)
include(qxtglobalshortcut/qxtglobalshortcut.pri)
include(forms/forms.pri)
include(update/update.pri)
include(localstorage/localstorage.pri)
include(settingsset/settingsset.pri)

SOURCES +=  main.cpp \
            common.cpp \
            singleapplication.cpp \
            log.cpp


HEADERS  += common.h \
            singleapplication.h \
            log.h



RESOURCES += \
            client.qrc


macx {
	TARGET = "NDR Client"
	LIBS += -framework SystemConfiguration -framework CoreFoundation -framework Security -framework Carbon
	ICON = icons/logo.icns
}
unix:!macx{
	QT += dbus
}

win32 {
    RC_FILE +=  $$PWD/appicon_win/appicon.rc
    LIBS +=     -lws2_32
}

CODEFORTR = UTF-8
DEFAULTCODEC = UTF-8
CODEC = UTF-8

TRANSLATIONS += ./ts-file/en_US.ts \
                ./ts-file/ja_JP.ts \
                ./ts-file/ko_KR.ts
