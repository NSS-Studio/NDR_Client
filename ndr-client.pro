TEMPLATE = subdirs

SUBDIRS += $$PWD/client/client.pro

CONFIG += ordered

CONFIG(debug, debug|release):{
    CONFIG += console #终端可作为调试输出载体
    win32 {
        LIBS += -lDbgHelp
    }
    QMAKE_LFLAGS_RELEASE = /DEBUG
}
else:CONFIG(release, debug|release):{
    DEFINES += QT_NO_DEBUG_OUTPUT #调试输入开关
}

CONFIG += c++17 warn_on exceptions
