#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T09:54:01
#
#-------------------------------------------------

QT       = core network

TARGET = auth
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++1z

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS     +=  $$PWD/authenticat.h \
                $$PWD/checkfilethread.h \
                $$PWD/verifythread.h \
                $$PWD/log.h

SOURCES     +=  $$PWD/authenticat.cpp \
                $$PWD/checkfilethread.cpp \
                $$PWD/verifythread.cpp \
                $$PWD/log.cpp
