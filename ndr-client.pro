TEMPLATE = subdirs

SUBDIRS += $$PWD/auth/auth.pro \
           $$PWD/client/client.pro

CONFIG += ordered

QT += c++14

QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG

CONFIG(debug, debug|release):{
    CONFIG += exceptions
}

