#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T08:30:29
#
#-------------------------------------------------

QT  += core network widgets xml quick svg quickcontrols2
TARGET = ndr-client
TEMPLATE = app

include(pppoe/pppoe.pri)
include(qmlmanager/newforms.pri)
include(update/update.pri)
include(localstorage/localstorage.pri)
include(settingsset/settingsset.pri)
include(auth/auth.pri)
include(3rd_party/Qt-AES/qaesencryption.pro)

SOURCES +=  main.cpp \
            ndrapplication.cpp \
            utils.cpp \
            resourcemanager.cpp

HEADERS  += ndrapplication.hpp \
            utils.hpp \
            ndrconfig.hpp \
            resourcemanager.hpp

RESOURCES += client.qrc

macx {
	TARGET = "NDR Client"
	ICON = icons/logo.icns
        SOURCES += $$PWD/utils_log_unix.cpp
}

unix:!macx{
        SOURCES += $$PWD/utils_log_unix.cpp
}

win32 {
    QMAKE_CXXFLAGS += -D_WIN32_WINNT=0x0601 # Windows 7
    LIBS += -lDbghelp
    RC_FILE +=  $$PWD/appicon_win/appicon.rc    \
                $$PWD/appicon_win/ndr.rc

    DISTFILES += $$PWD/appicon_win/ndr.rc    \
                 $$PWD/appicon_win/ndr-client.exe.manifest

    SOURCES += $$PWD/utils_netfix.cpp    \
               $$PWD/utils_log_win.cpp

    HEADERS += $$PWD/utils_netfix.hpp
}

CODEFORTR = UTF-8
DEFAULTCODEC = UTF-8
CODEC = UTF-8

TRANSLATIONS += ./ts-file/en_US.ts \
                ./ts-file/ja_JP.ts \
                ./ts-file/ko_KR.ts
