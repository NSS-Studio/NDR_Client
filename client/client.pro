#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T08:30:29
#
#-------------------------------------------------

QT       += core network
TARGET = ndr-client #目标文件
TEMPLATE = app

#CONFIG += console #终端可作为调试输出载体
#DEFINES += QT_NO_DEBUG_OUTPUT #调试输入开关
include(pppoe/pppoe.pri)
include(qxtglobalshortcut/qxtglobalshortcut.pri)
include(forms/forms.pri)
include(update/update.pri)
include(localstorage/localstorage.pri)
include(settingsset/settingsset.pri)
include(information/information.pri)
include(httpsoperation/httpsoperation.pri)

SOURCES +=  main.cpp \
            common.cpp \
            singleapplication.cpp \
            log.cpp


HEADERS  += common.h \
            singleapplication.h \
            log.h



RESOURCES += client.qrc


macx {
	TARGET = "NDR Client"
	ICON = icons/logo.icns
}
unix:!macx{

}

win32 {
    QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x500
    RC_FILE +=  $$PWD/appicon_win/appicon.rc
}

CODEFORTR = UTF-8
DEFAULTCODEC = UTF-8
CODEC = UTF-8

TRANSLATIONS += ./ts-file/en_US.ts \
                ./ts-file/ja_JP.ts \
                ./ts-file/ko_KR.ts
