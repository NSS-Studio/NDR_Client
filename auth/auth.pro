#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T09:54:01
#
#-------------------------------------------------

QT       = core network

TARGET = auth
TEMPLATE = lib
CONFIG += staticlib

#SOURCES += authenticat.cpp

#HEADERS += authenticat.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS     +=  $$PWD/authenticat.h \
                $$PWD/checkfilethread.h \
                $$PWD/verifythread.h \
                $$PWD/log.h \
                $$PWD/confusion.h

SOURCES     +=  $$PWD/authenticat.cpp \
                $$PWD/checkfilethread.cpp \
                $$PWD/verifythread.cpp \
                $$PWD/log.cpp \
                $$PWD/confusion.cpp
