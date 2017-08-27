TEMPLATE = subdirs

SUBDIRS += $$PWD/auth/auth.pro \
           $$PWD/client/client.pro

CONFIG += ordered

QT += c++14

CONFIG(debug, debug|release):{
    QMAKE_LFLAGS_RELEASE = /DEBUG
    CONFIG += exceptions
}

