#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T08:30:29
#
#-------------------------------------------------

QT       += core network widgets xml
TARGET = ndr-client #目标文件
TEMPLATE = app
CONFIG += c++1z

CONFIG(debug, debug|release):{
    # CONFIG += console #终端可作为调试输出载体
    win32 {
        LIBS += -lDbgHelp
    }
}
else:CONFIG(release, debug|release):{
    DEFINES += QT_NO_DEBUG_OUTPUT #调试输入开关
}

include(pppoe/pppoe.pri)
include(forms/forms.pri)
include(update/update.pri)
include(localstorage/localstorage.pri)
include(settingsset/settingsset.pri)
include(infomodule/infomodule.pri)

#不用UIC 因为可能导致更新UI 实际引用错误的遗忘BUG
#指定UI_DIR可以避免此问题 qmake会自动生成
UI_DIR=./forms

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
